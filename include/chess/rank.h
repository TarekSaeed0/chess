#ifndef CHESS_RANK_H_INCLUDED
#define CHESS_RANK_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/types.h>

void chess_rank_debug(ChessRank rank);
static inline bool chess_rank_is_valid(ChessRank rank) {
	return (rank & 0x8U) == 0x8U;
}
size_t chess_rank_from_algebraic(ChessRank *rank, const char *string);
size_t chess_rank_to_algebraic(ChessRank rank, char *string, size_t string_size);

#ifdef __cplusplus
}
#endif

#endif // CHESS_RANK_H_INCLUDED
