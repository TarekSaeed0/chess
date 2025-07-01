#include <chess/move.h>
#include <chess/move_private.h>

#include <chess/castling_rights.h>
#include <chess/color.h>
#include <chess/file.h>
#include <chess/macros_private.h>
#include <chess/moves.h>
#include <chess/offset.h>
#include <chess/piece.h>
#include <chess/piece_type.h>
#include <chess/position.h>
#include <chess/position_counter.h>
#include <chess/position_private.h>
#include <chess/rank.h>
#include <chess/square.h>

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void chess_move_debug(ChessMove move) {
	printf("(ChessMove) {\n");

	printf("\t.from = ");
	chess_square_debug(move.from);
	printf(",\n");

	printf("\t.to = ");
	chess_square_debug(move.to);
	printf(",\n");

	printf("\t.promotion_type = ");
	chess_piece_type_debug(move.promotion_type);
	printf(",\n");

	printf("\t.captured_piece = ");
	chess_piece_debug(move.captured_piece);
	printf(",\n");

	printf("\t.previous_castling_rights = ");
	chess_castling_rights_debug(move.previous_castling_rights);
	printf(",\n");

	printf("\t.previous_en_passant_square = ");
	chess_square_debug(move.previous_en_passant_square);
	printf(",\n");

	printf("\t.previous_half_move_clock = %u,\n", move.previous_half_move_clock);

	printf("}");
}
bool chess_move_is_valid(ChessMove move) {
	if (!chess_square_is_valid(move.from) || !chess_square_is_valid(move.to)) {
		return false;
	}

	if (move.promotion_type != CHESS_PIECE_TYPE_NONE && (move.promotion_type < CHESS_PIECE_TYPE_KNIGHT || move.promotion_type > CHESS_PIECE_TYPE_QUEEN)) {
		return false;
	}

	if ((!chess_piece_is_valid(move.captured_piece) && move.captured_piece != CHESS_PIECE_NONE) || chess_piece_type(move.captured_piece) == CHESS_PIECE_TYPE_KING) {
		return false;
	}

	if (!chess_castling_rights_is_valid(move.previous_castling_rights)) {
		return false;
	}

	if (!chess_square_is_valid(move.previous_en_passant_square) && move.previous_en_passant_square != CHESS_SQUARE_NONE) {
		return false;
	}

	return true;
}
ChessMove chess_move_new(const ChessPosition *position, ChessSquare from, ChessSquare to, ChessPieceType promotion_type) {
	assert(chess_position_is_valid(position) && chess_square_is_valid(from) && chess_square_is_valid(to) && (chess_piece_type_is_valid(promotion_type) || promotion_type == CHESS_PIECE_TYPE_NONE));

	ChessMove move = {
		.from                       = from,
		.to                         = to,
		.promotion_type             = promotion_type,
		.captured_piece             = position->board[to],
		.previous_castling_rights   = position->castling_rights,
		.previous_en_passant_square = position->en_passant_square,
		.previous_half_move_clock   = position->half_move_clock,
	};

	if (chess_move_is_en_passant(position, move)) {
		ChessOffset direction = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_OFFSET_NORTH : CHESS_OFFSET_SOUTH;
		move.captured_piece   = position->board[to - direction];
	}

	assert(chess_move_is_valid(move));

	return move;
}
size_t chess_move_from_algebraic(const ChessPosition *position, ChessMove *move, const char *string) {
	assert(chess_position_is_valid(position) && move != CHESS_NULL && string != CHESS_NULL);

	size_t total_read = 0;

	while (isspace(string[total_read])) {
		total_read++;
	}

	if (strncmp(&string[total_read], "O-O", 3) == 0) {
		ChessSquare from = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_SQUARE_E1 : CHESS_SQUARE_E8;

		if (chess_piece_type(position->board[from]) != CHESS_PIECE_TYPE_KING) {
			return 0;
		}

		*move = (ChessMove){
			.from                       = from,
			.to                         = (ChessSquare)(from + 2 * CHESS_OFFSET_EAST),
			.promotion_type             = CHESS_PIECE_TYPE_NONE,
			.captured_piece             = CHESS_PIECE_NONE,
			.previous_castling_rights   = position->castling_rights,
			.previous_en_passant_square = position->en_passant_square,
			.previous_half_move_clock   = position->half_move_clock,
		};
		total_read += 3;

		assert(chess_move_is_valid(*move));

		return total_read;
	}
	if (strncmp(&string[total_read], "O-O-O", 5) == 0) {
		ChessSquare from = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_SQUARE_E1 : CHESS_SQUARE_E8;

		if (chess_piece_type(position->board[from]) != CHESS_PIECE_TYPE_KING) {
			return 0;
		}

		*move = (ChessMove){
			.from                       = from,
			.to                         = (ChessSquare)(from + 2 * CHESS_OFFSET_WEST),
			.promotion_type             = CHESS_PIECE_TYPE_NONE,
			.captured_piece             = CHESS_PIECE_NONE,
			.previous_castling_rights   = position->castling_rights,
			.previous_en_passant_square = position->en_passant_square,
			.previous_half_move_clock   = position->half_move_clock,
		};
		total_read += 5;

		assert(chess_move_is_valid(*move));

		return total_read;
	}

	ChessPieceType type = CHESS_PIECE_TYPE_PAWN;
	total_read += chess_piece_type_from_algebraic(&type, &string[total_read]);

	ChessFile file = CHESS_FILE_NONE;
	ChessRank rank = CHESS_RANK_NONE;
	total_read += chess_file_from_algebraic(&file, &string[total_read]);
	total_read += chess_rank_from_algebraic(&rank, &string[total_read]);

	bool is_capture = false;
	if (string[total_read] == 'x') {
		is_capture = true;
		total_read++;
	}

	ChessSquare to = CHESS_SQUARE_NONE;
	size_t read    = chess_square_from_algebraic(&to, &string[total_read]);
	if (read != 0) {
		total_read += read;
	} else if (file != CHESS_FILE_NONE && rank != CHESS_RANK_NONE && !is_capture) {
		to   = chess_square_new(file, rank);
		file = CHESS_FILE_NONE;
		rank = CHESS_RANK_NONE;
	} else {
		return 0;
	}

	ChessPieceType promotion_type = CHESS_PIECE_TYPE_NONE;
	if (string[total_read] == '=') {
		total_read++;
		CHESS_READ(chess_piece_type_from_algebraic, &promotion_type);

		if (promotion_type < CHESS_PIECE_TYPE_KNIGHT || promotion_type > CHESS_PIECE_TYPE_QUEEN) {
			return 0;
		}
	}

	bool is_check     = false;
	bool is_checkmate = false;
	if (string[total_read] == '+') {
		is_check = true;
		total_read++;
	} else if (string[total_read] == '#') {
		is_checkmate = true;
		total_read++;
	}

	if (file != CHESS_FILE_NONE && rank != CHESS_RANK_NONE) {
		ChessSquare from = chess_square_new(file, rank);

		if (type != CHESS_PIECE_TYPE_NONE && chess_piece_type(position->board[from]) != type) {
			return 0;
		}

		*move = (ChessMove){
			.from                       = from,
			.to                         = to,
			.promotion_type             = promotion_type,
			.captured_piece             = position->board[to],
			.previous_castling_rights   = position->castling_rights,
			.previous_en_passant_square = position->en_passant_square,
			.previous_half_move_clock   = position->half_move_clock,
		};
		if (chess_move_is_en_passant(position, *move)) {
			ChessOffset direction = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_OFFSET_NORTH : CHESS_OFFSET_SOUTH;
			move->captured_piece  = position->board[to - direction];
		}

		if (is_capture && !chess_move_is_capture(position, *move)) {
			return 0;
		}

		if (is_check || is_checkmate) {
			if (!chess_move_is_legal(position, *move)) {
				return 0;
			}

			ChessPosition position_after_move = *position;
			chess_move_do_unchecked(&position_after_move, *move);

			if (is_check && !chess_position_is_check(&position_after_move)) {
				return 0;
			}

			if (is_checkmate && !chess_position_is_checkmate(&position_after_move)) {
				return 0;
			}
		}

		assert(chess_move_is_valid(*move));

		return total_read;
	}

	ChessMoves moves = chess_moves_generate(position);
	size_t matches   = 0;
	for (size_t i = 0; i < moves.count; i++) {
		if (moves.moves[i].to != to) {
			continue;
		}

		if (file != CHESS_FILE_NONE && chess_square_file(moves.moves[i].from) != file) {
			continue;
		}
		if (rank != CHESS_RANK_NONE && chess_square_rank(moves.moves[i].from) != rank) {
			continue;
		}

		if (chess_piece_type(position->board[moves.moves[i].from]) != type) {
			continue;
		}

		if (moves.moves[i].promotion_type != promotion_type) {
			continue;
		}

		if (is_capture && !chess_move_is_capture(position, moves.moves[i])) {
			continue;
		}

		if (is_check || is_checkmate) {
			ChessPosition position_after_move = *position;
			chess_move_do_unchecked(&position_after_move, moves.moves[i]);

			if (is_check && !chess_position_is_check(&position_after_move)) {
				continue;
			}

			if (is_checkmate && !chess_position_is_checkmate(&position_after_move)) {
				continue;
			}
		}

		*move = moves.moves[i];
		matches++;
	}

	if (matches != 1) {
		return 0;
	}

	assert(chess_move_is_valid(*move));

	return total_read;
}
size_t chess_move_to_algebraic(const ChessPosition *position, ChessMove move, char *string, size_t string_size) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	size_t total_written = 0;

	if (chess_move_is_kingside_castling(position, move)) {
		CHESS_WRITE_FORMATTED("O-O");
		return total_written;
	}
	if (chess_move_is_queenside_castling(position, move)) {
		CHESS_WRITE_FORMATTED("O-O-O");
		return total_written;
	}

	ChessPiece piece    = position->board[move.from];
	ChessPieceType type = chess_piece_type(piece);
	ChessFile file      = chess_square_file(move.from);
	if (type != CHESS_PIECE_TYPE_PAWN) {
		CHESS_WRITE(chess_piece_type_to_algebraic, type);

		ChessRank rank         = chess_square_rank(move.from);

		bool is_ambiguous      = false;
		bool is_file_ambiguous = false;
		bool is_rank_ambiguous = false;

		ChessMoves moves       = chess_moves_generate(position);
		for (size_t i = 0; i < moves.count; i++) {
			if (moves.moves[i].to != move.to || moves.moves[i].from == move.from || position->board[moves.moves[i].from] != piece) {
				continue;
			}

			is_ambiguous = true;

			if (chess_square_file(moves.moves[i].from) == file) {
				is_file_ambiguous = true;
			}
			if (chess_square_rank(moves.moves[i].from) == rank) {
				is_rank_ambiguous = true;
			}
		}
		if (is_ambiguous) {
			if (!is_file_ambiguous) {
				CHESS_WRITE(chess_file_to_algebraic, file);
			} else if (!is_rank_ambiguous) {
				CHESS_WRITE(chess_rank_to_algebraic, rank);
			} else {
				CHESS_WRITE(chess_file_to_algebraic, file);
				CHESS_WRITE(chess_rank_to_algebraic, rank);
			}
		}
	}

	if (chess_move_is_capture(position, move)) {
		if (type == CHESS_PIECE_TYPE_PAWN) {
			CHESS_WRITE(chess_file_to_algebraic, file);
		}
		CHESS_WRITE_FORMATTED("x");
	}

	CHESS_WRITE(chess_square_to_algebraic, move.to);

	if (chess_move_is_promotion(position, move)) {
		CHESS_WRITE_FORMATTED("=");
		CHESS_WRITE(chess_piece_type_to_algebraic, move.promotion_type);
	}

	if (chess_move_is_legal(position, move)) {
		ChessPosition position_after_move = *position;
		chess_move_do_unchecked(&position_after_move, move);
		if (chess_position_is_check(&position_after_move)) {
			if (chess_position_is_checkmate(&position_after_move)) {
				CHESS_WRITE_FORMATTED("#");
			} else {
				CHESS_WRITE_FORMATTED("+");
			}
		}
	}

	return total_written;
}
bool chess_move_is_pseudolegal(const ChessPosition *position, ChessMove move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	if (move.previous_en_passant_square != position->en_passant_square ||
	    move.previous_castling_rights != position->castling_rights ||
	    move.previous_half_move_clock != position->half_move_clock) {
		return false;
	}

	ChessPiece piece = position->board[move.from];

	ChessColor color = chess_piece_color(piece);
	if (color != position->side_to_move) {
		return false;
	}

	static CHESS_CONSTEXPR uint8_t moves[256] = {
		[CHESS_OFFSET_NORTH + 0x77]                         = 1U << CHESS_PIECE_TYPE_PAWN | 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
		[2 * CHESS_OFFSET_NORTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_PAWN | 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[3 * CHESS_OFFSET_NORTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[4 * CHESS_OFFSET_NORTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[5 * CHESS_OFFSET_NORTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[6 * CHESS_OFFSET_NORTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[7 * CHESS_OFFSET_NORTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,

		[CHESS_OFFSET_EAST + 0x77]                          = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
		[2 * CHESS_OFFSET_EAST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
		[3 * CHESS_OFFSET_EAST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[4 * CHESS_OFFSET_EAST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[5 * CHESS_OFFSET_EAST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[6 * CHESS_OFFSET_EAST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[7 * CHESS_OFFSET_EAST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,

		[CHESS_OFFSET_SOUTH + 0x77]                         = 1U << CHESS_PIECE_TYPE_PAWN | 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
		[2 * CHESS_OFFSET_SOUTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_PAWN | 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[3 * CHESS_OFFSET_SOUTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[4 * CHESS_OFFSET_SOUTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[5 * CHESS_OFFSET_SOUTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[6 * CHESS_OFFSET_SOUTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[7 * CHESS_OFFSET_SOUTH + 0x77]                     = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,

		[CHESS_OFFSET_WEST + 0x77]                          = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
		[2 * CHESS_OFFSET_WEST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
		[3 * CHESS_OFFSET_WEST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[4 * CHESS_OFFSET_WEST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[5 * CHESS_OFFSET_WEST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[6 * CHESS_OFFSET_WEST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,
		[7 * CHESS_OFFSET_WEST + 0x77]                      = 1U << CHESS_PIECE_TYPE_ROOK | 1U << CHESS_PIECE_TYPE_QUEEN,

		[CHESS_OFFSET_NORTH_EAST + 0x77]                    = 1U << CHESS_PIECE_TYPE_PAWN | 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
		[2 * CHESS_OFFSET_NORTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[3 * CHESS_OFFSET_NORTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[4 * CHESS_OFFSET_NORTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[5 * CHESS_OFFSET_NORTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[6 * CHESS_OFFSET_NORTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[7 * CHESS_OFFSET_NORTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,

		[CHESS_OFFSET_SOUTH_EAST + 0x77]                    = 1U << CHESS_PIECE_TYPE_PAWN | 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
		[2 * CHESS_OFFSET_SOUTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[3 * CHESS_OFFSET_SOUTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[4 * CHESS_OFFSET_SOUTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[5 * CHESS_OFFSET_SOUTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[6 * CHESS_OFFSET_SOUTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[7 * CHESS_OFFSET_SOUTH_EAST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,

		[CHESS_OFFSET_SOUTH_WEST + 0x77]                    = 1U << CHESS_PIECE_TYPE_PAWN | 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
		[2 * CHESS_OFFSET_SOUTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[3 * CHESS_OFFSET_SOUTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[4 * CHESS_OFFSET_SOUTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[5 * CHESS_OFFSET_SOUTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[6 * CHESS_OFFSET_SOUTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[7 * CHESS_OFFSET_SOUTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,

		[CHESS_OFFSET_NORTH_WEST + 0x77]                    = 1U << CHESS_PIECE_TYPE_PAWN | 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN | 1U << CHESS_PIECE_TYPE_KING,
		[2 * CHESS_OFFSET_NORTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[3 * CHESS_OFFSET_NORTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[4 * CHESS_OFFSET_NORTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[5 * CHESS_OFFSET_NORTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[6 * CHESS_OFFSET_NORTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,
		[7 * CHESS_OFFSET_NORTH_WEST + 0x77]                = 1U << CHESS_PIECE_TYPE_BISHOP | 1U << CHESS_PIECE_TYPE_QUEEN,

		[CHESS_OFFSET_WEST + 2 * CHESS_OFFSET_SOUTH + 0x77] = 1U << CHESS_PIECE_TYPE_KNIGHT,
		[CHESS_OFFSET_EAST + 2 * CHESS_OFFSET_SOUTH + 0x77] = 1U << CHESS_PIECE_TYPE_KNIGHT,
		[2 * CHESS_OFFSET_WEST + CHESS_OFFSET_SOUTH + 0x77] = 1U << CHESS_PIECE_TYPE_KNIGHT,
		[2 * CHESS_OFFSET_EAST + CHESS_OFFSET_SOUTH + 0x77] = 1U << CHESS_PIECE_TYPE_KNIGHT,
		[2 * CHESS_OFFSET_WEST + CHESS_OFFSET_NORTH + 0x77] = 1U << CHESS_PIECE_TYPE_KNIGHT,
		[2 * CHESS_OFFSET_EAST + CHESS_OFFSET_NORTH + 0x77] = 1U << CHESS_PIECE_TYPE_KNIGHT,
		[CHESS_OFFSET_WEST + 2 * CHESS_OFFSET_NORTH + 0x77] = 1U << CHESS_PIECE_TYPE_KNIGHT,
		[CHESS_OFFSET_EAST + 2 * CHESS_OFFSET_NORTH + 0x77] = 1U << CHESS_PIECE_TYPE_KNIGHT,
	};

	ChessOffset difference = move.to - move.from;
	uint8_t index          = (uint8_t)(difference + 0x77);

	ChessPieceType type    = chess_piece_type(piece);
	if (!(moves[index] & (1U << type))) {
		return false;
	}

	switch (type) {
		case CHESS_PIECE_TYPE_PAWN: {
			ChessOffset direction    = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_OFFSET_NORTH : CHESS_OFFSET_SOUTH;
			ChessRank promotion_rank = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_RANK_8 : CHESS_RANK_1;

			if (difference == direction) {
				return position->board[move.to] == CHESS_PIECE_NONE &&
				       !(chess_square_rank(move.to) == promotion_rank && move.promotion_type == CHESS_PIECE_TYPE_NONE);
			}

			if (difference == 2 * direction) {
				ChessRank start_rank = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_RANK_2 : CHESS_RANK_7;
				return chess_square_rank(move.from) == start_rank &&
				       position->board[move.from + direction] == CHESS_PIECE_NONE &&
				       position->board[move.to] == CHESS_PIECE_NONE;
			}

			if (difference == direction + CHESS_OFFSET_EAST || difference == direction + CHESS_OFFSET_WEST) {
				return move.to == position->en_passant_square ||
				       (chess_piece_color(position->board[move.to]) == chess_color_opposite(position->side_to_move) &&
				        !(chess_square_rank(move.to) == promotion_rank && move.promotion_type == CHESS_PIECE_TYPE_NONE));
			}

			return false;
		} break;
		case CHESS_PIECE_TYPE_KNIGHT: {
			return chess_piece_color(position->board[move.to]) != position->side_to_move;
		} break;
		case CHESS_PIECE_TYPE_BISHOP:
		case CHESS_PIECE_TYPE_ROOK:
		case CHESS_PIECE_TYPE_QUEEN: {
			if (chess_piece_color(position->board[move.to]) == position->side_to_move) {
				return false;
			}

			static CHESS_CONSTEXPR ChessOffset directions[256] = {
				[CHESS_OFFSET_NORTH + 0x77]          = CHESS_OFFSET_NORTH,
				[2 * CHESS_OFFSET_NORTH + 0x77]      = CHESS_OFFSET_NORTH,
				[3 * CHESS_OFFSET_NORTH + 0x77]      = CHESS_OFFSET_NORTH,
				[4 * CHESS_OFFSET_NORTH + 0x77]      = CHESS_OFFSET_NORTH,
				[5 * CHESS_OFFSET_NORTH + 0x77]      = CHESS_OFFSET_NORTH,
				[6 * CHESS_OFFSET_NORTH + 0x77]      = CHESS_OFFSET_NORTH,
				[7 * CHESS_OFFSET_NORTH + 0x77]      = CHESS_OFFSET_NORTH,

				[CHESS_OFFSET_EAST + 0x77]           = CHESS_OFFSET_EAST,
				[2 * CHESS_OFFSET_EAST + 0x77]       = CHESS_OFFSET_EAST,
				[3 * CHESS_OFFSET_EAST + 0x77]       = CHESS_OFFSET_EAST,
				[4 * CHESS_OFFSET_EAST + 0x77]       = CHESS_OFFSET_EAST,
				[5 * CHESS_OFFSET_EAST + 0x77]       = CHESS_OFFSET_EAST,
				[6 * CHESS_OFFSET_EAST + 0x77]       = CHESS_OFFSET_EAST,
				[7 * CHESS_OFFSET_EAST + 0x77]       = CHESS_OFFSET_EAST,

				[CHESS_OFFSET_SOUTH + 0x77]          = CHESS_OFFSET_SOUTH,
				[2 * CHESS_OFFSET_SOUTH + 0x77]      = CHESS_OFFSET_SOUTH,
				[3 * CHESS_OFFSET_SOUTH + 0x77]      = CHESS_OFFSET_SOUTH,
				[4 * CHESS_OFFSET_SOUTH + 0x77]      = CHESS_OFFSET_SOUTH,
				[5 * CHESS_OFFSET_SOUTH + 0x77]      = CHESS_OFFSET_SOUTH,
				[6 * CHESS_OFFSET_SOUTH + 0x77]      = CHESS_OFFSET_SOUTH,
				[7 * CHESS_OFFSET_SOUTH + 0x77]      = CHESS_OFFSET_SOUTH,

				[CHESS_OFFSET_WEST + 0x77]           = CHESS_OFFSET_WEST,
				[2 * CHESS_OFFSET_WEST + 0x77]       = CHESS_OFFSET_WEST,
				[3 * CHESS_OFFSET_WEST + 0x77]       = CHESS_OFFSET_WEST,
				[4 * CHESS_OFFSET_WEST + 0x77]       = CHESS_OFFSET_WEST,
				[5 * CHESS_OFFSET_WEST + 0x77]       = CHESS_OFFSET_WEST,
				[6 * CHESS_OFFSET_WEST + 0x77]       = CHESS_OFFSET_WEST,
				[7 * CHESS_OFFSET_WEST + 0x77]       = CHESS_OFFSET_WEST,

				[CHESS_OFFSET_NORTH_EAST + 0x77]     = CHESS_OFFSET_NORTH_EAST,
				[2 * CHESS_OFFSET_NORTH_EAST + 0x77] = CHESS_OFFSET_NORTH_EAST,
				[3 * CHESS_OFFSET_NORTH_EAST + 0x77] = CHESS_OFFSET_NORTH_EAST,
				[4 * CHESS_OFFSET_NORTH_EAST + 0x77] = CHESS_OFFSET_NORTH_EAST,
				[5 * CHESS_OFFSET_NORTH_EAST + 0x77] = CHESS_OFFSET_NORTH_EAST,
				[6 * CHESS_OFFSET_NORTH_EAST + 0x77] = CHESS_OFFSET_NORTH_EAST,
				[7 * CHESS_OFFSET_NORTH_EAST + 0x77] = CHESS_OFFSET_NORTH_EAST,

				[CHESS_OFFSET_SOUTH_EAST + 0x77]     = CHESS_OFFSET_SOUTH_EAST,
				[2 * CHESS_OFFSET_SOUTH_EAST + 0x77] = CHESS_OFFSET_SOUTH_EAST,
				[3 * CHESS_OFFSET_SOUTH_EAST + 0x77] = CHESS_OFFSET_SOUTH_EAST,
				[4 * CHESS_OFFSET_SOUTH_EAST + 0x77] = CHESS_OFFSET_SOUTH_EAST,
				[5 * CHESS_OFFSET_SOUTH_EAST + 0x77] = CHESS_OFFSET_SOUTH_EAST,
				[6 * CHESS_OFFSET_SOUTH_EAST + 0x77] = CHESS_OFFSET_SOUTH_EAST,
				[7 * CHESS_OFFSET_SOUTH_EAST + 0x77] = CHESS_OFFSET_SOUTH_EAST,

				[CHESS_OFFSET_SOUTH_WEST + 0x77]     = CHESS_OFFSET_SOUTH_WEST,
				[2 * CHESS_OFFSET_SOUTH_WEST + 0x77] = CHESS_OFFSET_SOUTH_WEST,
				[3 * CHESS_OFFSET_SOUTH_WEST + 0x77] = CHESS_OFFSET_SOUTH_WEST,
				[4 * CHESS_OFFSET_SOUTH_WEST + 0x77] = CHESS_OFFSET_SOUTH_WEST,
				[5 * CHESS_OFFSET_SOUTH_WEST + 0x77] = CHESS_OFFSET_SOUTH_WEST,
				[6 * CHESS_OFFSET_SOUTH_WEST + 0x77] = CHESS_OFFSET_SOUTH_WEST,
				[7 * CHESS_OFFSET_SOUTH_WEST + 0x77] = CHESS_OFFSET_SOUTH_WEST,

				[CHESS_OFFSET_NORTH_WEST + 0x77]     = CHESS_OFFSET_NORTH_WEST,
				[2 * CHESS_OFFSET_NORTH_WEST + 0x77] = CHESS_OFFSET_NORTH_WEST,
				[3 * CHESS_OFFSET_NORTH_WEST + 0x77] = CHESS_OFFSET_NORTH_WEST,
				[4 * CHESS_OFFSET_NORTH_WEST + 0x77] = CHESS_OFFSET_NORTH_WEST,
				[5 * CHESS_OFFSET_NORTH_WEST + 0x77] = CHESS_OFFSET_NORTH_WEST,
				[6 * CHESS_OFFSET_NORTH_WEST + 0x77] = CHESS_OFFSET_NORTH_WEST,
				[7 * CHESS_OFFSET_NORTH_WEST + 0x77] = CHESS_OFFSET_NORTH_WEST,
			};

			ChessOffset direction = directions[index];
			ChessSquare current   = (ChessSquare)(move.from + direction);
			while (true) {
				if (current == move.to) {
					return true;
				}
				if (position->board[current] != CHESS_PIECE_NONE) {
					return false;
				}
				current += direction;
			}
		} break;
		case CHESS_PIECE_TYPE_KING: {
			if (difference == 2 * CHESS_OFFSET_EAST) {
				ChessCastlingRights castling_right = position->side_to_move == CHESS_COLOR_WHITE
				                                         ? CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE
				                                         : CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE;

				ChessColor other_side              = chess_color_opposite(position->side_to_move);

				return (position->castling_rights & castling_right) &&
				       position->board[move.from + CHESS_OFFSET_EAST] == CHESS_PIECE_NONE &&
				       position->board[move.from + 2 * CHESS_OFFSET_EAST] == CHESS_PIECE_NONE &&
				       !chess_square_is_attacked(position, move.from, other_side) &&
				       !chess_square_is_attacked(position, (ChessSquare)(move.from + CHESS_OFFSET_EAST), other_side) &&
				       !chess_square_is_attacked(position, (ChessSquare)(move.from + 2 * CHESS_OFFSET_EAST), other_side);
			}

			if (difference == 2 * CHESS_OFFSET_WEST) {
				ChessCastlingRights castling_right = position->side_to_move == CHESS_COLOR_WHITE
				                                         ? CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE
				                                         : CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE;

				ChessColor other_side              = chess_color_opposite(position->side_to_move);

				return (position->castling_rights & castling_right) &&
				       position->board[move.from + CHESS_OFFSET_WEST] == CHESS_PIECE_NONE &&
				       position->board[move.from + 2 * CHESS_OFFSET_WEST] == CHESS_PIECE_NONE &&
				       position->board[move.from + 3 * CHESS_OFFSET_WEST] == CHESS_PIECE_NONE &&
				       !chess_square_is_attacked(position, move.from, other_side) &&
				       !chess_square_is_attacked(position, (ChessSquare)(move.from + CHESS_OFFSET_WEST), other_side) &&
				       !chess_square_is_attacked(position, (ChessSquare)(move.from + 2 * CHESS_OFFSET_WEST), other_side);
			}

			return chess_piece_color(position->board[move.to]) != position->side_to_move;
		} break;
		default: assert(false);
	}

	return true;
}
bool chess_move_is_legal(const ChessPosition *position, ChessMove move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	if (!chess_move_is_pseudolegal(position, move)) {
		return false;
	}

	ChessPosition position_after_move = *position;
	chess_move_do_unchecked(&position_after_move, move);
	return !chess_position_is_king_attacked(&position_after_move, position->side_to_move);
}
bool chess_move_is_promotion(const ChessPosition *position, ChessMove move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	(void)position;

	return move.promotion_type != CHESS_PIECE_TYPE_NONE;
}
bool chess_move_is_en_passant(const ChessPosition *position, ChessMove move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	return chess_piece_type(position->board[move.from]) == CHESS_PIECE_TYPE_PAWN &&
	       move.to == position->en_passant_square;
}
bool chess_move_is_capture(const ChessPosition *position, ChessMove move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	(void)position;

	return move.captured_piece != CHESS_PIECE_NONE;
}
bool chess_move_is_kingside_castling(const ChessPosition *position, ChessMove move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	return chess_piece_type(position->board[move.from]) == CHESS_PIECE_TYPE_KING &&
	       move.to - move.from == 2 * CHESS_OFFSET_EAST;
}
bool chess_move_is_queenside_castling(const ChessPosition *position, ChessMove move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	return chess_piece_type(position->board[move.from]) == CHESS_PIECE_TYPE_KING && move.to - move.from == 2 * CHESS_OFFSET_WEST;
}
void chess_move_do_unchecked(ChessPosition *position, ChessMove move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	ChessPiece moving_piece       = position->board[move.from];
	ChessPieceType moving_type    = chess_piece_type(moving_piece);

	position->board[move.to]      = moving_piece;
	position->board[move.from]    = CHESS_PIECE_NONE;

	ChessSquare en_passant_square = position->en_passant_square;
	position->en_passant_square   = CHESS_SQUARE_NONE;

	if (move.captured_piece != CHESS_PIECE_NONE) {
		position->half_move_clock = 0;
	} else {
		position->half_move_clock++;
	}

	if (position->side_to_move == CHESS_COLOR_BLACK) {
		position->full_move_number++;
	}

	if (move.promotion_type != CHESS_PIECE_TYPE_NONE) {
		position->board[move.to] = chess_piece_new(position->side_to_move, move.promotion_type);
	}

	if (moving_type == CHESS_PIECE_TYPE_PAWN) {
		position->half_move_clock = 0;

		ChessOffset direction     = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_OFFSET_NORTH : CHESS_OFFSET_SOUTH;
		if (move.to - move.from == 2 * direction) {
			position->en_passant_square = (ChessSquare)(move.from + direction);
		} else if (move.to == en_passant_square) {
			position->board[move.to - direction] = CHESS_PIECE_NONE;
		}
	}

	if (moving_type == CHESS_PIECE_TYPE_KING) {
		position->king_squares[chess_piece_color(moving_piece)] = move.to;

		if (move.to - move.from == 2 * CHESS_OFFSET_EAST) {
			position->board[move.to + CHESS_OFFSET_WEST] = position->board[move.to + CHESS_OFFSET_EAST];
			position->board[move.to + CHESS_OFFSET_EAST] = CHESS_PIECE_NONE;
		} else if (move.to - move.from == 2 * CHESS_OFFSET_WEST) {
			position->board[move.to + CHESS_OFFSET_EAST]     = position->board[move.to + 2 * CHESS_OFFSET_WEST];
			position->board[move.to + 2 * CHESS_OFFSET_WEST] = CHESS_PIECE_NONE;
		}

		position->castling_rights &= ~(position->side_to_move == CHESS_COLOR_WHITE ? CHESS_CASTLING_RIGHTS_WHITE : CHESS_CASTLING_RIGHTS_BLACK);
	}

	if (moving_type == CHESS_PIECE_TYPE_ROOK) {
		switch (move.from) {
			case CHESS_SQUARE_H1: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE); break;
			case CHESS_SQUARE_A1: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE); break;
			case CHESS_SQUARE_H8: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE); break;
			case CHESS_SQUARE_A8: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE); break;
			default:;
		}
	}
	if (chess_piece_type(move.captured_piece) == CHESS_PIECE_TYPE_ROOK) {
		switch (move.to) {
			case CHESS_SQUARE_H1: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE); break;
			case CHESS_SQUARE_A1: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE); break;
			case CHESS_SQUARE_H8: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE); break;
			case CHESS_SQUARE_A8: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE); break;
			default:;
		}
	}

	position->side_to_move = chess_color_opposite(position->side_to_move);
}
bool chess_move_do(ChessPosition *position, ChessMove move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	if (!chess_move_is_legal(position, move)) {
		return false;
	}

	chess_move_do_unchecked(position, move);

	chess_position_counter_increment(&position->position_counter, position);

	assert(chess_position_is_valid(position));

	return true;
}
void chess_move_undo_unchecked(ChessPosition *position, ChessMove move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	position->side_to_move      = chess_color_opposite(position->side_to_move);
	position->castling_rights   = move.previous_castling_rights;
	position->en_passant_square = move.previous_en_passant_square;
	position->half_move_clock   = move.previous_half_move_clock;

	if (position->side_to_move == CHESS_COLOR_BLACK) {
		position->full_move_number--;
	}

	ChessPiece moving_piece    = position->board[move.to];
	ChessPieceType moving_type = chess_piece_type(moving_piece);

	position->board[move.from] = moving_piece;
	position->board[move.to]   = move.captured_piece;

	if (move.promotion_type != CHESS_PIECE_TYPE_NONE) {
		position->board[move.to] = chess_piece_new(position->side_to_move, CHESS_PIECE_TYPE_PAWN);
	}

	if (moving_type == CHESS_PIECE_TYPE_PAWN && move.to == position->en_passant_square) {
		ChessOffset direction                = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_OFFSET_NORTH : CHESS_OFFSET_SOUTH;
		position->board[move.to - direction] = move.captured_piece;
		position->board[move.to]             = CHESS_PIECE_NONE;
	}

	if (moving_type == CHESS_PIECE_TYPE_KING) {
		position->king_squares[chess_piece_color(moving_piece)] = move.from;

		if (move.to - move.from == 2 * CHESS_OFFSET_EAST) {
			position->board[move.to + CHESS_OFFSET_EAST] = position->board[move.to + CHESS_OFFSET_WEST];
			position->board[move.to + CHESS_OFFSET_WEST] = CHESS_PIECE_NONE;
		} else if (move.to - move.from == 2 * CHESS_OFFSET_WEST) {
			position->board[move.to + CHESS_OFFSET_WEST]     = position->board[move.to + 2 * CHESS_OFFSET_EAST];
			position->board[move.to + 2 * CHESS_OFFSET_EAST] = CHESS_PIECE_NONE;
		}
	}
}
bool chess_move_undo(ChessPosition *position, ChessMove move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	ChessPosition position_before_move = *position;
	chess_move_undo_unchecked(&position_before_move, move);

	if (!chess_position_is_valid(&position_before_move) || !chess_move_is_legal(&position_before_move, move)) {
		return false;
	}

	*position = position_before_move;

	chess_position_counter_decrement(&position->position_counter, position);

	return true;
}
