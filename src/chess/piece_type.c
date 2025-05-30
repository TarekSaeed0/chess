#include <chess/piece_type.h>

#include <assert.h>
#include <stdio.h>

void chess_piece_type_debug(ChessPieceType type) {
	switch (type) {
		case CHESS_PIECE_TYPE_NONE:
			printf("CHESS_PIECE_TYPE_NONE");
			break;
		case CHESS_PIECE_TYPE_PAWN:
			printf("CHESS_PIECE_TYPE_PAWN");
			break;
		case CHESS_PIECE_TYPE_KNIGHT:
			printf("CHESS_PIECE_TYPE_KNIGHT");
			break;
		case CHESS_PIECE_TYPE_BISHOP:
			printf("CHESS_PIECE_TYPE_BISHOP");
			break;
		case CHESS_PIECE_TYPE_ROOK:
			printf("CHESS_PIECE_TYPE_ROOK");
			break;
		case CHESS_PIECE_TYPE_QUEEN:
			printf("CHESS_PIECE_TYPE_QUEEN");
			break;
		case CHESS_PIECE_TYPE_KING:
			printf("CHESS_PIECE_TYPE_KING");
			break;
		default:
			printf("(ChessPieceType)%d", type);
			break;
	}
}
bool chess_piece_type_is_valid(ChessPieceType type) {
	switch (type) {
		case CHESS_PIECE_TYPE_PAWN:
		case CHESS_PIECE_TYPE_KNIGHT:
		case CHESS_PIECE_TYPE_BISHOP:
		case CHESS_PIECE_TYPE_ROOK:
		case CHESS_PIECE_TYPE_QUEEN:
		case CHESS_PIECE_TYPE_KING: return true;
		default: return false;
	}
}
size_t chess_piece_type_from_algebraic(ChessPieceType *type, const char *string) {
	assert(type != CHESS_NULL && string != CHESS_NULL);

	switch (string[0]) {
		case 'P': *type = CHESS_PIECE_TYPE_PAWN; break;
		case 'N': *type = CHESS_PIECE_TYPE_KNIGHT; break;
		case 'B': *type = CHESS_PIECE_TYPE_BISHOP; break;
		case 'R': *type = CHESS_PIECE_TYPE_ROOK; break;
		case 'Q': *type = CHESS_PIECE_TYPE_QUEEN; break;
		case 'K': *type = CHESS_PIECE_TYPE_KING; break;
		default: return 0;
	}

	return 1;
}
size_t chess_piece_type_to_algebraic(ChessPieceType type, char *string, size_t string_size) {
	assert(chess_piece_type_is_valid(type) && (string != CHESS_NULL || string_size == 0));

	if (string != CHESS_NULL && string_size >= 2) {
		switch (type) {
			case CHESS_PIECE_TYPE_PAWN: string[0] = 'P'; break;
			case CHESS_PIECE_TYPE_KNIGHT: string[0] = 'N'; break;
			case CHESS_PIECE_TYPE_BISHOP: string[0] = 'B'; break;
			case CHESS_PIECE_TYPE_ROOK: string[0] = 'R'; break;
			case CHESS_PIECE_TYPE_QUEEN: string[0] = 'Q'; break;
			case CHESS_PIECE_TYPE_KING: string[0] = 'K'; break;
			default: assert(false);
		}
		string[1] = '\0';
	}

	return 1;
}
