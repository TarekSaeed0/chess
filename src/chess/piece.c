#include <chess/piece.h>

#include <assert.h>
#include <stdio.h>

void chess_piece_debug(ChessPiece piece) {
	switch (piece) {
		case CHESS_PIECE_NONE:
			printf("CHESS_PIECE_NONE");
			break;
		case CHESS_PIECE_WHITE_PAWN:
			printf("CHESS_PIECE_WHITE_PAWN");
			break;
		case CHESS_PIECE_WHITE_KNIGHT:
			printf("CHESS_PIECE_WHITE_KNIGHT");
			break;
		case CHESS_PIECE_WHITE_BISHOP:
			printf("CHESS_PIECE_WHITE_BISHOP");
			break;
		case CHESS_PIECE_WHITE_ROOK:
			printf("CHESS_PIECE_WHITE_ROOK");
			break;
		case CHESS_PIECE_WHITE_QUEEN:
			printf("CHESS_PIECE_WHITE_QUEEN");
			break;
		case CHESS_PIECE_WHITE_KING:
			printf("CHESS_PIECE_WHITE_KING");
			break;
		case CHESS_PIECE_BLACK_PAWN:
			printf("CHESS_PIECE_BLACK_PAWN");
			break;
		case CHESS_PIECE_BLACK_KNIGHT:
			printf("CHESS_PIECE_BLACK_KNIGHT");
			break;
		case CHESS_PIECE_BLACK_BISHOP:
			printf("CHESS_PIECE_BLACK_BISHOP");
			break;
		case CHESS_PIECE_BLACK_ROOK:
			printf("CHESS_PIECE_BLACK_ROOK");
			break;
		case CHESS_PIECE_BLACK_QUEEN:
			printf("CHESS_PIECE_BLACK_QUEEN");
			break;
		case CHESS_PIECE_BLACK_KING:
			printf("CHESS_PIECE_BLACK_KING");
			break;
		default:
			printf("(ChessPiece)%d", piece);
			break;
	}
}
bool chess_piece_is_valid(ChessPiece piece) {
	switch (piece) {
		case CHESS_PIECE_WHITE_PAWN:
		case CHESS_PIECE_WHITE_KNIGHT:
		case CHESS_PIECE_WHITE_BISHOP:
		case CHESS_PIECE_WHITE_ROOK:
		case CHESS_PIECE_WHITE_QUEEN:
		case CHESS_PIECE_WHITE_KING:
		case CHESS_PIECE_BLACK_PAWN:
		case CHESS_PIECE_BLACK_KNIGHT:
		case CHESS_PIECE_BLACK_BISHOP:
		case CHESS_PIECE_BLACK_ROOK:
		case CHESS_PIECE_BLACK_QUEEN:
		case CHESS_PIECE_BLACK_KING: return true;
		default: return false;
	}
}
size_t chess_piece_from_algebraic(ChessPiece *piece, const char *string) {
	assert(piece != CHESS_NULL && string != CHESS_NULL);

	switch (string[0]) {
		case 'P': *piece = CHESS_PIECE_WHITE_PAWN; break;
		case 'N': *piece = CHESS_PIECE_WHITE_KNIGHT; break;
		case 'B': *piece = CHESS_PIECE_WHITE_BISHOP; break;
		case 'R': *piece = CHESS_PIECE_WHITE_ROOK; break;
		case 'Q': *piece = CHESS_PIECE_WHITE_QUEEN; break;
		case 'K': *piece = CHESS_PIECE_WHITE_KING; break;
		case 'p': *piece = CHESS_PIECE_BLACK_PAWN; break;
		case 'n': *piece = CHESS_PIECE_BLACK_KNIGHT; break;
		case 'b': *piece = CHESS_PIECE_BLACK_BISHOP; break;
		case 'r': *piece = CHESS_PIECE_BLACK_ROOK; break;
		case 'q': *piece = CHESS_PIECE_BLACK_QUEEN; break;
		case 'k': *piece = CHESS_PIECE_BLACK_KING; break;
		default: return 0;
	}

	return 1;
}
size_t chess_piece_to_algebraic(ChessPiece piece, char *string, size_t string_size) {
	assert(chess_piece_is_valid(piece) && (string != CHESS_NULL || string_size == 0));

	if (string != CHESS_NULL && string_size >= 2) {
		switch (piece) {
			case CHESS_PIECE_WHITE_PAWN: string[0] = 'P'; break;
			case CHESS_PIECE_WHITE_KNIGHT: string[0] = 'N'; break;
			case CHESS_PIECE_WHITE_BISHOP: string[0] = 'B'; break;
			case CHESS_PIECE_WHITE_ROOK: string[0] = 'R'; break;
			case CHESS_PIECE_WHITE_QUEEN: string[0] = 'Q'; break;
			case CHESS_PIECE_WHITE_KING: string[0] = 'K'; break;
			case CHESS_PIECE_BLACK_PAWN: string[0] = 'p'; break;
			case CHESS_PIECE_BLACK_KNIGHT: string[0] = 'n'; break;
			case CHESS_PIECE_BLACK_BISHOP: string[0] = 'b'; break;
			case CHESS_PIECE_BLACK_ROOK: string[0] = 'r'; break;
			case CHESS_PIECE_BLACK_QUEEN: string[0] = 'q'; break;
			case CHESS_PIECE_BLACK_KING: string[0] = 'k'; break;
			default: assert(false);
		}
		string[1] = '\0';
	}

	return 1;
}
