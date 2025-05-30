#include <chess/move.h>

#include <chess/color.h>
#include <chess/file.h>
#include <chess/macros_private.h>
#include <chess/moves.h>
#include <chess/offset.h>
#include <chess/piece.h>
#include <chess/piece_type.h>
#include <chess/position.h>
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

	printf("}");
}
bool chess_move_is_valid(ChessMove move) {
	if (!chess_square_is_valid(move.from) || !chess_square_is_valid(move.to)) {
		return false;
	}

	switch (move.promotion_type) {
		case CHESS_PIECE_TYPE_NONE:
		case CHESS_PIECE_TYPE_KNIGHT:
		case CHESS_PIECE_TYPE_BISHOP:
		case CHESS_PIECE_TYPE_ROOK:
		case CHESS_PIECE_TYPE_QUEEN: return true;
		default: return false;
	}
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

		*move = (ChessMove){ .from = from, .to = (ChessSquare)(from + 2 * CHESS_OFFSET_EAST) };
		total_read += 3;

		return total_read;
	}
	if (strncmp(&string[total_read], "O-O-O", 5) == 0) {
		ChessSquare from = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_SQUARE_E1 : CHESS_SQUARE_E8;

		if (chess_piece_type(position->board[from]) != CHESS_PIECE_TYPE_KING) {
			return 0;
		}

		*move = (ChessMove){ .from = from, .to = (ChessSquare)(from + 2 * CHESS_OFFSET_WEST) };
		total_read += 5;

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
		READ(chess_piece_type_from_algebraic, &promotion_type);
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
			.from           = from,
			.to             = to,
			.promotion_type = promotion_type,
		};

		if (is_capture && !chess_move_is_capture(position, *move)) {
			return 0;
		}

		if (is_check || is_checkmate) {
			ChessPosition position_after_move = *position;
			if (!chess_move_do(&position_after_move, *move)) {
				return 0;
			}

			if (is_check && !chess_position_is_check(&position_after_move)) {
				return 0;
			}

			if (is_checkmate && !chess_position_is_checkmate(&position_after_move)) {
				return 0;
			}
		}

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
			if (!chess_move_do(&position_after_move, moves.moves[i])) {
				continue;
			}

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

	return total_read;
}
size_t chess_move_to_algebraic(const ChessPosition *position, ChessMove move, char *string, size_t string_size) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	size_t total_written = 0;

	if (chess_move_is_kingside_castling(position, move)) {
		WRITE_FORMATTED("O-O");
		return total_written;
	}
	if (chess_move_is_queenside_castling(position, move)) {
		WRITE_FORMATTED("O-O-O");
		return total_written;
	}

	ChessPiece piece    = position->board[move.from];
	ChessPieceType type = chess_piece_type(piece);
	ChessFile file      = chess_square_file(move.from);
	if (type != CHESS_PIECE_TYPE_PAWN) {
		WRITE(chess_piece_type_to_algebraic, type);

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
				WRITE(chess_file_to_algebraic, file);
			} else if (!is_rank_ambiguous) {
				WRITE(chess_rank_to_algebraic, rank);
			} else {
				WRITE(chess_file_to_algebraic, file);
				WRITE(chess_rank_to_algebraic, rank);
			}
		}
	}

	if (chess_move_is_capture(position, move)) {
		if (type == CHESS_PIECE_TYPE_PAWN) {
			WRITE(chess_file_to_algebraic, file);
		}
		WRITE_FORMATTED("x");
	}

	WRITE(chess_square_to_algebraic, move.to);

	if (chess_move_is_promotion(position, move)) {
		WRITE_FORMATTED("=");
		WRITE(chess_piece_type_to_algebraic, move.promotion_type);
	}

	ChessPosition position_after_move = *position;
	if (chess_move_do(&position_after_move, move)) {
		if (chess_position_is_check(&position_after_move)) {
			if (chess_position_is_checkmate(&position_after_move)) {
				WRITE_FORMATTED("#");
			} else {
				WRITE_FORMATTED("+");
			}
		}
	}

	return total_written;
}
bool chess_move_is_legal(const ChessPosition *position, ChessMove move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	// TODO: implement actual move legality checking
	ChessMoves moves = chess_moves_generate(position);
	for (size_t i = 0; i < moves.count; i++) {
		if (moves.moves[i].from == move.from && moves.moves[i].to == move.to &&
		    moves.moves[i].promotion_type == move.promotion_type) {
			return true;
		}
	}

	return false;
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

	if (position->board[move.to] != CHESS_PIECE_NONE) {
		return true;
	}

	return chess_move_is_en_passant(position, move);
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

	ChessPiece piece              = position->board[move.from];
	ChessPieceType type           = chess_piece_type(piece);
	ChessPiece captured_piece     = position->board[move.to];
	ChessPieceType captured_type  = chess_piece_type(captured_piece);
	ChessColor side_to_move       = position->side_to_move;
	ChessSquare en_passant_square = position->en_passant_square;

	position->board[move.to]      = piece;
	position->board[move.from]    = CHESS_PIECE_NONE;

	position->side_to_move        = chess_color_opposite(side_to_move);

	position->en_passant_square   = CHESS_SQUARE_NONE;

	if (captured_type != CHESS_PIECE_TYPE_NONE) {
		position->half_move_clock = 0;
	} else {
		position->half_move_clock++;
	}

	if (side_to_move == CHESS_COLOR_BLACK) {
		position->full_move_number++;
	}

	if (move.promotion_type != CHESS_PIECE_TYPE_NONE) {
		position->board[move.to] = chess_piece_new(side_to_move, move.promotion_type);
	}

	if (type == CHESS_PIECE_TYPE_PAWN) {
		position->half_move_clock = 0;

		chess_offset direction    = side_to_move == CHESS_COLOR_WHITE ? CHESS_OFFSET_NORTH : CHESS_OFFSET_SOUTH;
		if (move.to - move.from == 2 * direction) {
			position->en_passant_square = (ChessSquare)(move.from + direction);
		} else if (move.to == en_passant_square) {
			position->board[move.to - direction] = CHESS_PIECE_NONE;
		}
	}

	if (type == CHESS_PIECE_TYPE_KING) {
		position->king_squares[chess_piece_color(piece)] = move.to;

		if (move.to - move.from == 2 * CHESS_OFFSET_EAST) {
			position->board[move.to + CHESS_OFFSET_WEST] = position->board[move.to + CHESS_OFFSET_EAST];
			position->board[move.to + CHESS_OFFSET_EAST] = CHESS_PIECE_NONE;
		} else if (move.to - move.from == 2 * CHESS_OFFSET_WEST) {
			position->board[move.to + CHESS_OFFSET_EAST]     = position->board[move.to + 2 * CHESS_OFFSET_WEST];
			position->board[move.to + 2 * CHESS_OFFSET_WEST] = CHESS_PIECE_NONE;
		}

		position->castling_rights &=
		    ~(side_to_move == CHESS_COLOR_WHITE
		          ? CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE | CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE
		          : CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE | CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE);
	}

	if (type == CHESS_PIECE_TYPE_ROOK) {
		switch (move.from) {
			case CHESS_SQUARE_H1: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE); break;
			case CHESS_SQUARE_A1: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE); break;
			case CHESS_SQUARE_H8: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE); break;
			case CHESS_SQUARE_A8: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE); break;
			default:;
		}
	}
	if (captured_type == CHESS_PIECE_TYPE_ROOK) {
		switch (move.to) {
			case CHESS_SQUARE_H1: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE); break;
			case CHESS_SQUARE_A1: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE); break;
			case CHESS_SQUARE_H8: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE); break;
			case CHESS_SQUARE_A8: position->castling_rights &= ~(CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE); break;
			default:;
		}
	}
}
bool chess_move_do(ChessPosition *position, ChessMove move) {
	assert(chess_position_is_valid(position) && chess_move_is_valid(move));

	if (!chess_move_is_legal(position, move)) {
		return false;
	}

	chess_move_do_unchecked(position, move);

	assert(chess_position_is_valid(position));

	return true;
}
