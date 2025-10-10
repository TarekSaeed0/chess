#include <chess/position.h>
#include <chess/position_private.h>

#include <chess/castling_rights.h>
#include <chess/color.h>
#include <chess/macros_private.h>
#include <chess/moves.h>
#include <chess/offset.h>
#include <chess/piece.h>
#include <chess/position_counter.h>
#include <chess/square.h>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

void chess_position_debug(const ChessPosition *position) {
	printf("(ChessPosition) {\n");

	printf("\t.board = {\n");
	for (ChessSquare square = CHESS_SQUARE_A1; square <= CHESS_SQUARE_H8; square++) {
		if (!chess_square_is_valid(square)) {
			square += CHESS_SQUARE_A2 - (CHESS_SQUARE_H1 + 1) - 1;
		}

		ChessPiece piece = position->board[square];
		if (piece == CHESS_PIECE_NONE) {
			continue;
		}
		printf("\t\t[");
		chess_square_debug(square);
		printf("] = ");
		chess_piece_debug(piece);
		printf(",\n");
	}
	printf("\t},\n");

	printf("\t.pieces = {\n");
	for (ChessColor color = CHESS_COLOR_WHITE; color <= CHESS_COLOR_BLACK; color++) {
		printf("\t\t[");
		chess_color_debug(color);
		printf("] = {\n");
		for (ChessPieceType type = CHESS_PIECE_TYPE_PAWN; type <= CHESS_PIECE_TYPE_KING; type++) {
			printf("\t\t\t[");
			chess_piece_type_debug(type);
			printf("] = {");
			for (size_t i = 0; i < position->piece_counts[color][type]; i++) {
				if (i > 0) {
					printf(", ");
				}
				chess_square_debug(position->pieces[color][type][i]);
			}
			printf("},\n");
		}
		printf("\t\t},\n");
	}
	printf("\t},\n");

	printf("\t.piece_counts = {\n");
	for (ChessColor color = CHESS_COLOR_WHITE; color <= CHESS_COLOR_BLACK; color++) {
		printf("\t\t[");
		chess_color_debug(color);
		printf("] = {\n");
		for (ChessPieceType type = CHESS_PIECE_TYPE_PAWN; type <= CHESS_PIECE_TYPE_KING; type++) {
			printf("\t\t\t[");
			chess_piece_type_debug(type);
			printf("] = %" PRIu8 ",\n", position->piece_counts[color][type]);
		}
		printf("\t\t},\n");
	}
	printf("\t},\n");

	printf("\t.piece_indices = {\n");
	for (ChessSquare square = CHESS_SQUARE_A1; square <= CHESS_SQUARE_H8; square++) {
		if (!chess_square_is_valid(square)) {
			square += CHESS_SQUARE_A2 - (CHESS_SQUARE_H1 + 1) - 1;
		}

		if (position->board[square] == CHESS_PIECE_NONE) {
			continue;
		}
		printf("\t\t[");
		chess_square_debug(square);
		printf("] = %" PRIu8 ",\n", position->piece_indices[square]);
	}
	printf("\t},\n");

	printf("\t.side_to_move = ");
	chess_color_debug(position->side_to_move);
	printf(",\n");

	printf("\t.castling_rights = ");
	chess_castling_rights_debug(position->castling_rights);
	printf(",\n");

	printf("\t.en_passant_square = ");
	chess_square_debug(position->en_passant_square);
	printf(",\n");

	printf("\t.half_move_clock = %u,\n", position->half_move_clock);

	printf("\t.full_move_number = %u,\n", position->full_move_number);

	printf("}");
}
bool chess_position_is_valid(const ChessPosition *position) {
	if (position == CHESS_NULL) {
		return false;
	}

	if (!chess_color_is_valid(position->side_to_move)) {
		return false;
	}

	if (!chess_castling_rights_is_valid(position->castling_rights) ||
	    ((position->castling_rights & CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE) &&
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

	uint8_t piece_counts[CHESS_COLOR_BLACK + 1][CHESS_PIECE_TYPE_KING + 1] = { 0 };

	for (ChessSquare square = CHESS_SQUARE_A1; square <= CHESS_SQUARE_H8; square++) {
		if (!chess_square_is_valid(square)) {
			square += CHESS_SQUARE_A2 - (CHESS_SQUARE_H1 + 1) - 1;
			continue;
		}

		ChessPiece piece = position->board[square];

		if (piece == CHESS_PIECE_NONE) {
			continue;
		}

		if (!chess_piece_is_valid(piece)) {
			return false;
		}

		ChessColor color    = chess_piece_color(piece);
		ChessPieceType type = chess_piece_type(piece);

		piece_counts[color][type]++;
	}

	for (ChessColor color = CHESS_COLOR_WHITE; color <= CHESS_COLOR_BLACK; color++) {
		for (ChessPieceType type = CHESS_PIECE_TYPE_PAWN; type <= CHESS_PIECE_TYPE_KING; type++) {
			if (piece_counts[color][type] != position->piece_counts[color][type]) {
				return false;
			}

			for (size_t i = 0; i < position->piece_counts[color][type]; i++) {
				ChessSquare square = position->pieces[color][type][i];

				if (position->piece_indices[square] != i) {
					return false;
				}

				if (!chess_square_is_valid(square) || position->board[square] != chess_piece_new(color, type)) {
					return false;
				}
			}
		}
	}

	return chess_position_counter_is_valid(&position->position_counter);
}
ChessPosition chess_position_new(void) {
	ChessPosition position = {
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

		    [CHESS_SQUARE_A6] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_B6] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_C6] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_D6] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_E6] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_F6] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_G6] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_H6] = CHESS_PIECE_NONE,

		    [CHESS_SQUARE_A5] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_B5] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_C5] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_D5] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_E5] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_F5] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_G5] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_H5] = CHESS_PIECE_NONE,

		    [CHESS_SQUARE_A4] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_B4] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_C4] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_D4] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_E4] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_F4] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_G4] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_H4] = CHESS_PIECE_NONE,

		    [CHESS_SQUARE_A3] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_B3] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_C3] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_D3] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_E3] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_F3] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_G3] = CHESS_PIECE_NONE,
		    [CHESS_SQUARE_H3] = CHESS_PIECE_NONE,

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
		.pieces = {
		    [CHESS_COLOR_WHITE] = {
		        [CHESS_PIECE_TYPE_PAWN] = {
		            CHESS_SQUARE_A2,
		            CHESS_SQUARE_B2,
		            CHESS_SQUARE_C2,
		            CHESS_SQUARE_D2,
		            CHESS_SQUARE_E2,
		            CHESS_SQUARE_F2,
		            CHESS_SQUARE_G2,
		            CHESS_SQUARE_H2,
		        },
		        [CHESS_PIECE_TYPE_KNIGHT] = {
		            CHESS_SQUARE_B1,
		            CHESS_SQUARE_G1,
		        },
		        [CHESS_PIECE_TYPE_BISHOP] = {
		            CHESS_SQUARE_C1,
		            CHESS_SQUARE_F1,
		        },
		        [CHESS_PIECE_TYPE_ROOK] = {
		            CHESS_SQUARE_A1,
		            CHESS_SQUARE_H1,
		        },
		        [CHESS_PIECE_TYPE_QUEEN] = {
		            CHESS_SQUARE_D1,
		        },
		        [CHESS_PIECE_TYPE_KING] = {
		            CHESS_SQUARE_E1,
		        },
		    },
		    [CHESS_COLOR_BLACK] = {
		        [CHESS_PIECE_TYPE_PAWN] = {
		            CHESS_SQUARE_A7,
		            CHESS_SQUARE_B7,
		            CHESS_SQUARE_C7,
		            CHESS_SQUARE_D7,
		            CHESS_SQUARE_E7,
		            CHESS_SQUARE_F7,
		            CHESS_SQUARE_G7,
		            CHESS_SQUARE_H7,
		        },
		        [CHESS_PIECE_TYPE_KNIGHT] = {
		            CHESS_SQUARE_B8,
		            CHESS_SQUARE_G8,
		        },
		        [CHESS_PIECE_TYPE_BISHOP] = {
		            CHESS_SQUARE_C8,
		            CHESS_SQUARE_F8,
		        },
		        [CHESS_PIECE_TYPE_ROOK] = {
		            CHESS_SQUARE_A8,
		            CHESS_SQUARE_H8,
		        },
		        [CHESS_PIECE_TYPE_QUEEN] = {
		            CHESS_SQUARE_D8,
		        },
		        [CHESS_PIECE_TYPE_KING] = {
		            CHESS_SQUARE_E8,
		        },
		    },
		},
		.piece_counts = {
		    [CHESS_COLOR_WHITE] = {
		        [CHESS_PIECE_TYPE_PAWN]   = 8,
		        [CHESS_PIECE_TYPE_KNIGHT] = 2,
		        [CHESS_PIECE_TYPE_BISHOP] = 2,
		        [CHESS_PIECE_TYPE_ROOK]   = 2,
		        [CHESS_PIECE_TYPE_QUEEN]  = 1,
		        [CHESS_PIECE_TYPE_KING]   = 1,
		    },
		    [CHESS_COLOR_BLACK] = {
		        [CHESS_PIECE_TYPE_PAWN]   = 8,
		        [CHESS_PIECE_TYPE_KNIGHT] = 2,
		        [CHESS_PIECE_TYPE_BISHOP] = 2,
		        [CHESS_PIECE_TYPE_ROOK]   = 2,
		        [CHESS_PIECE_TYPE_QUEEN]  = 1,
		        [CHESS_PIECE_TYPE_KING]   = 1,
		    },
		},
		.piece_indices = {
		    [CHESS_SQUARE_A2] = 0,
		    [CHESS_SQUARE_B2] = 1,
		    [CHESS_SQUARE_C2] = 2,
		    [CHESS_SQUARE_D2] = 3,
		    [CHESS_SQUARE_E2] = 4,
		    [CHESS_SQUARE_F2] = 5,
		    [CHESS_SQUARE_G2] = 6,
		    [CHESS_SQUARE_H2] = 7,

		    [CHESS_SQUARE_B1] = 0,
		    [CHESS_SQUARE_G1] = 1,

		    [CHESS_SQUARE_C1] = 0,
		    [CHESS_SQUARE_F1] = 1,

		    [CHESS_SQUARE_A1] = 0,
		    [CHESS_SQUARE_H1] = 1,

		    [CHESS_SQUARE_D1] = 0,

		    [CHESS_SQUARE_E1] = 0,

		    [CHESS_SQUARE_A7] = 0,
		    [CHESS_SQUARE_B7] = 1,
		    [CHESS_SQUARE_C7] = 2,
		    [CHESS_SQUARE_D7] = 3,
		    [CHESS_SQUARE_E7] = 4,
		    [CHESS_SQUARE_F7] = 5,
		    [CHESS_SQUARE_G7] = 6,
		    [CHESS_SQUARE_H7] = 7,

		    [CHESS_SQUARE_B8] = 0,
		    [CHESS_SQUARE_G8] = 1,

		    [CHESS_SQUARE_C8] = 0,
		    [CHESS_SQUARE_F8] = 1,

		    [CHESS_SQUARE_A8] = 0,
		    [CHESS_SQUARE_H8] = 1,

		    [CHESS_SQUARE_D8] = 0,

		    [CHESS_SQUARE_E8] = 0,
		},
		.side_to_move      = CHESS_COLOR_WHITE,
		.castling_rights   = CHESS_CASTLING_RIGHTS_ALL,
		.en_passant_square = CHESS_SQUARE_NONE,
		.half_move_clock   = 0,
		.full_move_number  = 1,
		.position_counter  = chess_position_counter_new(),
	};

	assert(chess_position_is_valid(&position));

	return position;
}
void chess_position_drop(ChessPosition *position) {
	assert(chess_position_is_valid(position));

	chess_position_counter_drop(&position->position_counter);
}
ChessPiece chess_position_piece_at_square(const ChessPosition *position, ChessSquare square) {
	assert(chess_position_is_valid(position));
	assert(chess_square_is_valid(square));

	return position->board[square];
}
ChessColor chess_position_side_to_move(const ChessPosition *position) {
	assert(chess_position_is_valid(position));

	return position->side_to_move;
}
void chess_position_place_piece(ChessPosition *position, ChessPiece piece, ChessSquare square) {
	assert(chess_piece_is_valid(piece));
	assert(chess_square_is_valid(square));
	assert(position->board[square] == CHESS_PIECE_NONE);

	ChessColor color                                                   = chess_piece_color(piece);
	ChessPieceType type                                                = chess_piece_type(piece);

	position->board[square]                                            = piece;
	position->pieces[color][type][position->piece_counts[color][type]] = square;
	position->piece_indices[square]                                    = position->piece_counts[color][type]++;
}
ChessPiece chess_position_remove_piece(ChessPosition *position, ChessSquare square) {
	assert(chess_square_is_valid(square));
	assert(position->board[square] != CHESS_PIECE_NONE);

	ChessPiece piece                                                                            = position->board[square];
	ChessColor color                                                                            = chess_piece_color(piece);
	ChessPieceType type                                                                         = chess_piece_type(piece);

	position->board[square]                                                                     = CHESS_PIECE_NONE;
	position->pieces[color][type][position->piece_indices[square]]                              = position->pieces[color][type][--position->piece_counts[color][type]];
	position->piece_indices[position->pieces[color][type][position->piece_counts[color][type]]] = position->piece_indices[square];

	return piece;
}
void chess_position_move_piece(ChessPosition *position, ChessSquare from, ChessSquare to) {
	assert(chess_square_is_valid(from));
	assert(chess_square_is_valid(to));
	assert(position->board[from] != CHESS_PIECE_NONE);
	assert(position->board[to] == CHESS_PIECE_NONE);

	ChessPiece piece                                             = position->board[from];
	ChessColor color                                             = chess_piece_color(piece);
	ChessPieceType type                                          = chess_piece_type(piece);

	position->board[to]                                          = piece;
	position->board[from]                                        = CHESS_PIECE_NONE;
	position->pieces[color][type][position->piece_indices[from]] = to;
	position->piece_indices[to]                                  = position->piece_indices[from];
}
size_t chess_position_from_fen(ChessPosition *position, const char *string) {
	assert(position != CHESS_NULL);
	assert(string != CHESS_NULL);

	size_t total_read = 0;

	while (isspace(string[total_read])) {
		total_read++;
	}

	memset(position->board, CHESS_PIECE_NONE, sizeof(position->board));
	memset(position->pieces, CHESS_SQUARE_NONE, sizeof(position->pieces));
	memset(position->piece_counts, 0, sizeof(position->piece_counts));
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
				ChessPiece piece = CHESS_PIECE_NONE;
				CHESS_READ(chess_piece_from_algebraic, &piece);

				chess_position_place_piece(position, piece, square);
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
		CHESS_READ(chess_square_from_algebraic, &position->en_passant_square);
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
				CHESS_WRITE_FORMATTED("%c", (char)('0' + count));
			} else {
				CHESS_WRITE(chess_piece_to_algebraic, piece);
			}
		}
		if (rank != CHESS_RANK_1) {
			CHESS_WRITE_FORMATTED("/");
		}
	}

	CHESS_WRITE_FORMATTED(" ");

	CHESS_WRITE_FORMATTED("%c", position->side_to_move == CHESS_COLOR_WHITE ? 'w' : 'b');

	CHESS_WRITE_FORMATTED(" ");

	if (position->castling_rights) {
		if (position->castling_rights & CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE) {
			CHESS_WRITE_FORMATTED("K");
		}
		if (position->castling_rights & CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE) {
			CHESS_WRITE_FORMATTED("Q");
		}
		if (position->castling_rights & CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE) {
			CHESS_WRITE_FORMATTED("k");
		}
		if (position->castling_rights & CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE) {
			CHESS_WRITE_FORMATTED("q");
		}
	} else {
		CHESS_WRITE_FORMATTED("-");
	}

	CHESS_WRITE_FORMATTED(" ");

	if (position->en_passant_square != CHESS_SQUARE_NONE) {
		CHESS_WRITE(chess_square_to_algebraic, position->en_passant_square);
	} else {
		CHESS_WRITE_FORMATTED("-");
	}

	CHESS_WRITE_FORMATTED(" ");

	CHESS_WRITE_FORMATTED("%u %u", position->half_move_clock, position->full_move_number);

	return total_written;
}
bool chess_position_is_king_attacked(const ChessPosition *position, ChessColor color) {
	assert(chess_position_is_valid(position));

	return chess_square_is_attacked(position, position->pieces[color][CHESS_PIECE_TYPE_KING][0], chess_color_opposite(color));
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

	return chess_position_counter_count(&position->position_counter, position) >= 3;
}
bool chess_position_is_insufficient_material(const ChessPosition *position) {
	assert(chess_position_is_valid(position));

	if (position->piece_counts[CHESS_COLOR_WHITE][CHESS_PIECE_TYPE_PAWN] > 0 ||
	    position->piece_counts[CHESS_COLOR_BLACK][CHESS_PIECE_TYPE_PAWN] > 0 ||
	    position->piece_counts[CHESS_COLOR_WHITE][CHESS_PIECE_TYPE_ROOK] > 0 ||
	    position->piece_counts[CHESS_COLOR_BLACK][CHESS_PIECE_TYPE_ROOK] > 0 ||
	    position->piece_counts[CHESS_COLOR_WHITE][CHESS_PIECE_TYPE_QUEEN] > 0 ||
	    position->piece_counts[CHESS_COLOR_BLACK][CHESS_PIECE_TYPE_QUEEN] > 0) {
		return false;
	}

	unsigned int white_bishops           = position->piece_counts[CHESS_COLOR_WHITE][CHESS_PIECE_TYPE_BISHOP];
	unsigned int black_bishops           = position->piece_counts[CHESS_COLOR_BLACK][CHESS_PIECE_TYPE_BISHOP];
	unsigned int white_knights           = position->piece_counts[CHESS_COLOR_WHITE][CHESS_PIECE_TYPE_KNIGHT];
	unsigned int black_knights           = position->piece_counts[CHESS_COLOR_BLACK][CHESS_PIECE_TYPE_KNIGHT];

	ChessColor white_bishop_square_color = CHESS_COLOR_NONE;
	if (white_bishops > 0) {
		ChessSquare square        = position->pieces[CHESS_COLOR_WHITE][CHESS_PIECE_TYPE_BISHOP][0];
		white_bishop_square_color = chess_square_color(square);
	}

	ChessColor black_bishop_square_color = CHESS_COLOR_NONE;
	if (black_bishops > 0) {
		ChessSquare square        = position->pieces[CHESS_COLOR_BLACK][CHESS_PIECE_TYPE_BISHOP][0];
		black_bishop_square_color = chess_square_color(square);
	}

	if (white_bishops == 0 && black_bishops == 0 &&
	    white_knights == 0 && black_knights == 0) {
		return true;
	}

	if ((white_bishops == 1) + (white_knights == 1) + (black_bishops == 1) + (black_knights == 1) == 1) {
		return true;
	}

	if (white_bishops == 1 && black_bishops == 1 &&
	    white_knights == 0 && black_knights == 0 &&
	    white_bishop_square_color == black_bishop_square_color) {
		return true;
	}

	return false;
}
uint64_t chess_random(uint64_t *state) {
	assert(state != CHESS_NULL && *state != 0);

	uint64_t value = *state;
	value ^= value >> 12;
	value ^= value << 25;
	value ^= value >> 27;

	*state = value;

	return value * 0x2545F4914F6CDD1DULL;
}
uint64_t chess_position_hash(const ChessPosition *position) {
	assert(chess_position_is_valid(position));

	static bool initialized                                                               = false;
	static uint64_t piece_at_square_hash[CHESS_PIECE_BLACK_KING + 1][CHESS_SQUARE_H8 + 1] = { 0 };
	static uint64_t side_to_move_hash                                                     = 0;
	static uint64_t castling_rights_hash[CHESS_CASTLING_RIGHTS_ALL + 1]                   = { 0 };
	static uint64_t en_passant_file_hash[CHESS_FILE_H + 1]                                = { 0 };
	if (!initialized) {
		uint64_t random_state = 0x9E3779B97F4A7C15ULL;

		for (ChessPiece piece = CHESS_PIECE_WHITE_PAWN; piece <= CHESS_PIECE_BLACK_KING; piece++) {
			if (!chess_piece_is_valid(piece)) {
				piece = CHESS_PIECE_BLACK_PAWN - 1;
				continue;
			}

			for (ChessSquare square = CHESS_SQUARE_A1; square <= CHESS_SQUARE_H8; square++) {
				if (!chess_square_is_valid(square)) {
					square += CHESS_SQUARE_A2 - (CHESS_SQUARE_H1 + 1) - 1;
					continue;
				}

				piece_at_square_hash[piece][square] = chess_random(&random_state);
			}
		}

		side_to_move_hash = chess_random(&random_state);

		for (ChessCastlingRights castling_rights = CHESS_CASTLING_RIGHTS_NONE; castling_rights <= CHESS_CASTLING_RIGHTS_ALL; castling_rights++) {
			castling_rights_hash[castling_rights] = chess_random(&random_state);
		}

		for (ChessFile file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			en_passant_file_hash[file] = chess_random(&random_state);
		}

		initialized = true;
	}

	uint64_t hash = 0;

	for (ChessColor color = CHESS_COLOR_WHITE; color <= CHESS_COLOR_BLACK; color++) {
		for (ChessPieceType type = CHESS_PIECE_TYPE_PAWN; type <= CHESS_PIECE_TYPE_KING; type++) {
			for (size_t i = 0; i < position->piece_counts[color][type]; i++) {
				ChessSquare square = position->pieces[color][type][i];
				hash ^= piece_at_square_hash[position->board[square]][square];
			}
		}
	}

	if (position->side_to_move == CHESS_COLOR_BLACK) {
		hash ^= side_to_move_hash;
	}

	hash ^= castling_rights_hash[position->castling_rights];

	if (position->en_passant_square != CHESS_SQUARE_NONE) {
		ChessFile file = chess_square_file(position->en_passant_square);
		hash ^= en_passant_file_hash[file];
	}

	return hash;
}
double chess_position_evaluate(const ChessPosition *position) {
	assert(chess_position_is_valid(position));

	static const double piece_values[] = {
		[CHESS_PIECE_WHITE_PAWN]   = 1.0,
		[CHESS_PIECE_WHITE_KNIGHT] = 3.0,
		[CHESS_PIECE_WHITE_BISHOP] = 3.0,
		[CHESS_PIECE_WHITE_ROOK]   = 5.0,
		[CHESS_PIECE_WHITE_QUEEN]  = 9.0,
		[CHESS_PIECE_WHITE_KING]   = 200.0,

		[CHESS_PIECE_BLACK_PAWN]   = -1.0,
		[CHESS_PIECE_BLACK_KNIGHT] = -3.0,
		[CHESS_PIECE_BLACK_BISHOP] = -3.0,
		[CHESS_PIECE_BLACK_ROOK]   = -5.0,
		[CHESS_PIECE_BLACK_QUEEN]  = -9.0,
		[CHESS_PIECE_BLACK_KING]   = -200.0,

		[CHESS_PIECE_NONE]         = 0.0,
	};

	double value                                                           = 0.0;

	unsigned int pawn_file_counts[CHESS_COLOR_BLACK + 1][CHESS_FILE_H + 1] = { 0 };
	unsigned int weak_pawns[CHESS_COLOR_BLACK + 1]                         = { 0 };

	for (ChessColor color = CHESS_COLOR_WHITE; color <= CHESS_COLOR_BLACK; color++) {
		for (ChessPieceType type = CHESS_PIECE_TYPE_PAWN; type <= CHESS_PIECE_TYPE_KING; type++) {
			for (size_t i = 0; i < position->piece_counts[color][type]; i++) {
				ChessSquare square = position->pieces[color][type][i];
				ChessPiece piece   = position->board[square];

				value += piece_values[piece];

				if (type == CHESS_PIECE_TYPE_PAWN) {
					ChessFile file = chess_square_file(square);
					pawn_file_counts[color][file]++;

					ChessOffset direction = color == CHESS_COLOR_WHITE ? CHESS_OFFSET_NORTH : CHESS_OFFSET_SOUTH;
					if (chess_square_is_valid(square + direction) &&
					    position->board[square + direction] != CHESS_PIECE_NONE) {
						weak_pawns[color]++;
					}
				}
			}
		}
	}

	for (ChessColor color = CHESS_COLOR_WHITE; color <= CHESS_COLOR_BLACK; color++) {
		for (ChessFile file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			if (pawn_file_counts[color][file] == 0) {
				continue;
			}

			weak_pawns[color] += pawn_file_counts[color][file] - 1;

			if (!(file > CHESS_FILE_A && pawn_file_counts[color][file - 1] > 0) && !(file < CHESS_FILE_H && pawn_file_counts[color][file + 1] > 0)) {
				weak_pawns[color] += pawn_file_counts[color][file];
			}
		}
	}

	value -= 0.5 * weak_pawns[CHESS_COLOR_WHITE];
	value += 0.5 * weak_pawns[CHESS_COLOR_BLACK];

	ChessPosition temporary = *position;
	if (position->side_to_move == CHESS_COLOR_WHITE) {
		value += 0.1 * (double)chess_moves_generate(&temporary).count;
		temporary.side_to_move      = CHESS_COLOR_BLACK;
		temporary.en_passant_square = CHESS_SQUARE_NONE;
		value -= 0.1 * (double)chess_moves_generate(&temporary).count;
	} else {
		value -= 0.1 * (double)chess_moves_generate(&temporary).count;
		temporary.side_to_move      = CHESS_COLOR_WHITE;
		temporary.en_passant_square = CHESS_SQUARE_NONE;
		value += 0.1 * (double)chess_moves_generate(&temporary).count;
	}

	return value;
}
