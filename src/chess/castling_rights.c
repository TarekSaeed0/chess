#include <chess/castling_rights.h>

#include <assert.h>
#include <stdio.h>

void chess_castling_rights_debug(ChessCastlingRights rights) {
	if (!chess_castling_rights_is_valid(rights)) {
		printf("(ChessCastlingRights)%d", rights);
		return;
	}

	while (true) {
		if ((rights & CHESS_CASTLING_RIGHTS_ALL) == CHESS_CASTLING_RIGHTS_ALL) {
			printf("CHESS_CASTLING_RIGHTS_ALL");
			rights &= ~CHESS_CASTLING_RIGHTS_ALL;
		} else if ((rights & CHESS_CASTLING_RIGHTS_WHITE) == CHESS_CASTLING_RIGHTS_WHITE) {
			printf("CHESS_CASTLING_RIGHTS_WHITE");
			rights &= ~CHESS_CASTLING_RIGHTS_WHITE;
		} else if ((rights & CHESS_CASTLING_RIGHTS_BLACK) == CHESS_CASTLING_RIGHTS_BLACK) {
			printf("CHESS_CASTLING_RIGHTS_BLACK");
			rights &= ~CHESS_CASTLING_RIGHTS_BLACK;
		} else if ((rights & CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE) == CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE) {
			printf("CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE");
			rights &= ~CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE;
		} else if ((rights & CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE) == CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE) {
			printf("CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE");
			rights &= ~CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE;
		} else if ((rights & CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE) == CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE) {
			printf("CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE");
			rights &= ~CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE;
		} else if ((rights & CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE) == CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE) {
			printf("CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE");
			rights &= ~CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE;
		}

		if (rights == CHESS_CASTLING_RIGHTS_NONE) {
			break;
		}

		printf(" | ");
	}
}
bool chess_castling_rights_is_valid(ChessCastlingRights rights) {
	return (rights & CHESS_CASTLING_RIGHTS_ALL) == rights;
}
