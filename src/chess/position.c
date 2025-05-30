#include <chess/position.h>
#include <chess/position_private.h>

#include <chess/color.h>
#include <chess/macros_private.h>
#include <chess/moves.h>
#include <chess/offset.h>
#include <chess/piece.h>
#include <chess/square.h>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

bool chess_position_is_valid(const ChessPosition *position) {
	if (position == CHESS_NULL) {
		return false;
	}

	if (!chess_color_is_valid(position->side_to_move)) {
		return false;
	}

	if (!chess_square_is_valid(position->king_squares[CHESS_COLOR_WHITE]) ||
	    position->board[position->king_squares[CHESS_COLOR_WHITE]] != CHESS_PIECE_WHITE_KING ||
	    !chess_square_is_valid(position->king_squares[CHESS_COLOR_BLACK]) ||
	    position->board[position->king_squares[CHESS_COLOR_BLACK]] != CHESS_PIECE_BLACK_KING) {
		return false;
	}

	if (((position->castling_rights & CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE) &&
	     (position->board[CHESS_SQUARE_E1] != CHESS_PIECE_WHITE_KING ||
	      position->board[CHESS_SQUARE_H1] != CHESS_PIECE_WHITE_ROOK)) ||
	    ((position->castling_rights & CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE) &&
	     (position->board[CHESS_SQUARE_E1] != CHESS_PIECE_WHITE_KING ||
	      position->board[CHESS_SQUARE_A1] != CHESS_PIECE_WHITE_ROOK)) ||
	    ((position->castling_rights & CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE) &&
	     (position->board[CHESS_SQUARE_E8] != CHESS_PIECE_BLACK_KING ||
	      position->board[CHESS_SQUARE_H8] != CHESS_PIECE_BLACK_ROOK)) ||
	    ((position->castling_rights & CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE) &&
	     (position->board[CHESS_SQUARE_E8] != CHESS_PIECE_BLACK_KING ||
	      position->board[CHESS_SQUARE_A8] != CHESS_PIECE_BLACK_ROOK))) {
		return false;
	}

	if (position->en_passant_square != CHESS_SQUARE_NONE &&
	    (!chess_square_is_valid(position->en_passant_square) ||
	             position->board[position->en_passant_square] != CHESS_PIECE_NONE ||
	             position->side_to_move == CHESS_COLOR_WHITE
	         ? position->board[position->en_passant_square + CHESS_OFFSET_SOUTH] != CHESS_PIECE_BLACK_PAWN ||
	               position->board[position->en_passant_square + CHESS_OFFSET_NORTH] != CHESS_PIECE_NONE
	         : position->board[position->en_passant_square + CHESS_OFFSET_NORTH] != CHESS_PIECE_WHITE_PAWN ||
	               position->board[position->en_passant_square + CHESS_OFFSET_SOUTH] != CHESS_PIECE_NONE)) {
		return false;
	}

	for (ChessRank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		for (ChessFile file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			ChessSquare square = chess_square_new(file, rank);
			ChessPiece piece   = position->board[square];
			if (piece != CHESS_PIECE_NONE && !chess_piece_is_valid(piece)) {
				return false;
			}

			ChessColor color    = chess_piece_color(piece);
			ChessPieceType type = chess_piece_type(piece);
			if (type == CHESS_PIECE_TYPE_KING && square != position->king_squares[color]) {
				return false;
			}
		}
	}

	return true;
}
ChessPosition chess_position_new(void) {
	return (ChessPosition){
		.board = {
		    [CHESS_SQUARE_A8] = CHESS_PIECE_BLACK_ROOK,
		    [CHESS_SQUARE_B8] = CHESS_PIECE_BLACK_KNIGHT,
		    [CHESS_SQUARE_C8] = CHESS_PIECE_BLACK_BISHOP,
		    [CHESS_SQUARE_D8] = CHESS_PIECE_BLACK_QUEEN,
		    [CHESS_SQUARE_E8] = CHESS_PIECE_BLACK_KING,
		    [CHESS_SQUARE_F8] = CHESS_PIECE_BLACK_BISHOP,
		    [CHESS_SQUARE_G8] = CHESS_PIECE_BLACK_KNIGHT,
		    [CHESS_SQUARE_H8] = CHESS_PIECE_BLACK_ROOK,

		    [CHESS_SQUARE_A7] = CHESS_PIECE_BLACK_PAWN,
		    [CHESS_SQUARE_B7] = CHESS_PIECE_BLACK_PAWN,
		    [CHESS_SQUARE_C7] = CHESS_PIECE_BLACK_PAWN,
		    [CHESS_SQUARE_D7] = CHESS_PIECE_BLACK_PAWN,
		    [CHESS_SQUARE_E7] = CHESS_PIECE_BLACK_PAWN,
		    [CHESS_SQUARE_F7] = CHESS_PIECE_BLACK_PAWN,
		    [CHESS_SQUARE_G7] = CHESS_PIECE_BLACK_PAWN,
		    [CHESS_SQUARE_H7] = CHESS_PIECE_BLACK_PAWN,

		    [CHESS_SQUARE_A2] = CHESS_PIECE_WHITE_PAWN,
		    [CHESS_SQUARE_B2] = CHESS_PIECE_WHITE_PAWN,
		    [CHESS_SQUARE_C2] = CHESS_PIECE_WHITE_PAWN,
		    [CHESS_SQUARE_D2] = CHESS_PIECE_WHITE_PAWN,
		    [CHESS_SQUARE_E2] = CHESS_PIECE_WHITE_PAWN,
		    [CHESS_SQUARE_F2] = CHESS_PIECE_WHITE_PAWN,
		    [CHESS_SQUARE_G2] = CHESS_PIECE_WHITE_PAWN,
		    [CHESS_SQUARE_H2] = CHESS_PIECE_WHITE_PAWN,

		    [CHESS_SQUARE_A1] = CHESS_PIECE_WHITE_ROOK,
		    [CHESS_SQUARE_B1] = CHESS_PIECE_WHITE_KNIGHT,
		    [CHESS_SQUARE_C1] = CHESS_PIECE_WHITE_BISHOP,
		    [CHESS_SQUARE_D1] = CHESS_PIECE_WHITE_QUEEN,
		    [CHESS_SQUARE_E1] = CHESS_PIECE_WHITE_KING,
		    [CHESS_SQUARE_F1] = CHESS_PIECE_WHITE_BISHOP,
		    [CHESS_SQUARE_G1] = CHESS_PIECE_WHITE_KNIGHT,
		    [CHESS_SQUARE_H1] = CHESS_PIECE_WHITE_ROOK,
		},
		.side_to_move      = CHESS_COLOR_WHITE,
		.castling_rights   = CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE | CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE | CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE | CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE,
		.en_passant_square = CHESS_SQUARE_NONE,
		.half_move_clock   = 0,
		.full_move_number  = 1,
		.king_squares      = {
        [CHESS_COLOR_WHITE] = CHESS_SQUARE_E1,
        [CHESS_COLOR_BLACK] = CHESS_SQUARE_E8,
    }
	};
}
size_t chess_position_from_fen(ChessPosition *position, const char *string) {
	assert(position != CHESS_NULL && string != CHESS_NULL);

	size_t total_read = 0;

	while (isspace(string[total_read])) {
		total_read++;
	}

	memset(position->board, CHESS_PIECE_NONE, sizeof(position->board));
	for (ChessRank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		for (ChessFile file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			ChessSquare square = chess_square_new(file, rank);
			if ('1' <= string[total_read] && string[total_read] <= '8') {
				if (string[total_read] > CHESS_FILE_H - file + '1') {
					return 0;
				}
				file += (string[total_read] - '1');
				total_read++;
			} else {
				READ(chess_piece_from_algebraic, &position->board[square]);

				if (chess_piece_type(position->board[square]) == CHESS_PIECE_TYPE_KING) {
					position->king_squares[chess_piece_color(position->board[square])] = square;
				}
			}
		}
		if (rank != CHESS_RANK_1) {
			if (string[total_read] != '/') {
				return 0;
			}
			total_read++;
		}
	}

	if (!isspace(string[total_read])) {
		return 0;
	}
	while (isspace(string[total_read])) {
		total_read++;
	}

	if (string[total_read] == 'w') {
		position->side_to_move = CHESS_COLOR_WHITE;
	} else if (string[total_read] == 'b') {
		position->side_to_move = CHESS_COLOR_BLACK;
	} else {
		return 0;
	}
	total_read++;

	if (!isspace(string[total_read])) {
		return 0;
	}
	while (isspace(string[total_read])) {
		total_read++;
	}

	position->castling_rights = CHESS_CASTLING_RIGHTS_NONE;
	while (string[total_read] != '\0' && string[total_read] != ' ') {
		switch (string[total_read]) {
			case 'K': position->castling_rights |= CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE; break;
			case 'Q': position->castling_rights |= CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE; break;
			case 'k': position->castling_rights |= CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE; break;
			case 'q': position->castling_rights |= CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE; break;
			case '-': break;
			default: return 0;
		}
		total_read++;
	}

	if (!isspace(string[total_read])) {
		return 0;
	}
	while (isspace(string[total_read])) {
		total_read++;
	}

	if (string[total_read] == '-') {
		position->en_passant_square = CHESS_SQUARE_NONE;
		total_read++;
	} else {
		READ(chess_square_from_algebraic, &position->en_passant_square);
	}

	if (!isspace(string[total_read])) {
		return 0;
	}
	while (isspace(string[total_read])) {
		total_read++;
	}

	errno                = 0;
	char *end            = CHESS_NULL;
	unsigned long number = strtoul(string + total_read, &end, 10);
	if (end == string + total_read || errno != 0 || number > UINT_MAX) {
		return 0;
	}
	position->half_move_clock = (unsigned int)number;
	total_read                = (size_t)(end - string);

	if (!isspace(string[total_read])) {
		return 0;
	}
	while (isspace(string[total_read])) {
		total_read++;
	}

	errno  = 0;
	end    = CHESS_NULL;
	number = strtoul(string + total_read, &end, 10);
	if (end == string + total_read || errno != 0 || number > UINT_MAX) {
		return 0;
	}
	position->full_move_number = (unsigned int)number;
	total_read                 = (size_t)(end - string);

	return total_read;
}
size_t chess_position_to_fen(const ChessPosition *position, char *string, size_t string_size) {
	assert(chess_position_is_valid(position));

	size_t total_written = 0;

	for (ChessRank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		for (ChessFile file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			ChessPiece piece = position->board[chess_square_new(file, rank)];
			if (piece == CHESS_PIECE_NONE) {
				unsigned int count = 1;
				while (file < CHESS_FILE_H) {
					piece = position->board[chess_square_new(file + 1, rank)];
					if (piece != CHESS_PIECE_NONE) {
						break;
					}
					count++;
					file++;
				}
				WRITE_FORMATTED("%c", (char)('0' + count));
			} else {
				WRITE(chess_piece_to_algebraic, piece);
			}
		}
		if (rank != CHESS_RANK_1) {
			WRITE_FORMATTED("/");
		}
	}

	WRITE_FORMATTED(" ");

	WRITE_FORMATTED("%c", position->side_to_move == CHESS_COLOR_WHITE ? 'w' : 'b');

	WRITE_FORMATTED(" ");

	if (position->castling_rights) {
		if (position->castling_rights & CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE) {
			WRITE_FORMATTED("K");
		}
		if (position->castling_rights & CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE) {
			WRITE_FORMATTED("Q");
		}
		if (position->castling_rights & CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE) {
			WRITE_FORMATTED("k");
		}
		if (position->castling_rights & CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE) {
			WRITE_FORMATTED("q");
		}
	} else {
		WRITE_FORMATTED("-");
	}

	WRITE_FORMATTED(" ");

	if (position->en_passant_square != CHESS_SQUARE_NONE) {
		WRITE(chess_square_to_algebraic, position->en_passant_square);
	} else {
		WRITE_FORMATTED("-");
	}

	WRITE_FORMATTED(" ");

	WRITE_FORMATTED("%u %u", position->half_move_clock, position->full_move_number);

	return total_written;
}
bool chess_position_is_king_attacked(const ChessPosition *position, ChessColor color) {
	assert(chess_position_is_valid(position));

	return chess_square_is_attacked(position, position->king_squares[color], chess_color_opposite(color));
}
bool chess_position_is_check(const ChessPosition *position) {
	assert(chess_position_is_valid(position));

	return chess_position_is_king_attacked(position, position->side_to_move);
}
bool chess_position_is_checkmate(const ChessPosition *position) {
	assert(chess_position_is_valid(position));

	return chess_position_is_check(position) && chess_moves_generate(position).count == 0;
}
bool chess_position_is_stalemate(const ChessPosition *position) {
	assert(chess_position_is_valid(position));

	return !chess_position_is_check(position) && chess_moves_generate(position).count == 0;
}
bool chess_position_is_fifty_move_rule(const ChessPosition *position) {
	assert(chess_position_is_valid(position));

	return position->half_move_clock >= 100 && !chess_position_is_checkmate(position);
}
bool chess_position_is_threefold_repetition(const ChessPosition *position) {
	assert(chess_position_is_valid(position));

	(void)position;

	// TODO: implement threefold repetition detection

	return false;
}
bool chess_position_is_insufficient_material(const ChessPosition *position) {
	assert(chess_position_is_valid(position));

	(void)position;

	// TODO: implement insufficient material detection

	return false;
}
