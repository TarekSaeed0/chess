#ifndef CHESS_RANK_H_INCLUDED
#define CHESS_RANK_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/macros.h>

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
	#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

CHESS_ENUM(int8_t, ChessRank) {
	CHESS_RANK_1    = 0,
	CHESS_RANK_2    = 1,
	CHESS_RANK_3    = 2,
	CHESS_RANK_4    = 3,
	CHESS_RANK_5    = 4,
	CHESS_RANK_6    = 5,
	CHESS_RANK_7    = 6,
	CHESS_RANK_8    = 7,

	CHESS_RANK_NONE = 8,
};

void chess_rank_debug(ChessRank rank);
bool chess_rank_is_valid(ChessRank rank);
size_t chess_rank_from_algebraic(ChessRank *rank, const char *string);
size_t chess_rank_to_algebraic(ChessRank rank, char *string, size_t string_size);

#ifdef __cplusplus
}
#endif

#endif // CHESS_RANK_H_INCLUDED
