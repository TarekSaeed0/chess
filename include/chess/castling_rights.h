#ifndef CHESS_CASTLING_RIGHTS_H_INCLUDED
#define CHESS_CASTLING_RIGHTS_H_INCLUDED

#include <chess/macros.h>

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
	#include <stdbool.h>
#endif
#include <stdint.h>

CHESS_ENUM(uint8_t, ChessCastlingRights) {
	CHESS_CASTLING_RIGHTS_NONE            = 0U,

	CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE  = 1U << 0U,
	CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE = 1U << 1U,
	CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE  = 1U << 2U,
	CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE = 1U << 3U,

	CHESS_CASTLING_RIGHTS_WHITE           = CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE | CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE,
	CHESS_CASTLING_RIGHTS_BLACK           = CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE | CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE,

	CHESS_CASTLING_RIGHTS_ALL             = CHESS_CASTLING_RIGHTS_WHITE | CHESS_CASTLING_RIGHTS_BLACK,
};

void chess_castling_rights_debug(ChessCastlingRights rights);
bool chess_castling_rights_is_valid(ChessCastlingRights rights);

#endif // CHESS_CASTLING_RIGHTS_H_INCLUDED
