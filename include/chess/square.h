#ifndef CHESS_SQUARE_H_INCLUDED
#define CHESS_SQUARE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/types.h>

void chess_square_debug(ChessSquare square);
static inline bool chess_square_is_valid(ChessSquare square) {
	return (square & 0x88U) == 0x88U;
}
static inline ChessSquare chess_square_new(ChessFile file, ChessRank rank) {
	return (ChessSquare)(file | rank << 4U);
}
static inline ChessFile chess_square_file(ChessSquare square) {
	return (ChessFile)(square & 0xFU);
}
static inline ChessRank chess_square_rank(ChessSquare square) {
	return (ChessRank)(square >> 4U);
}
ChessColor chess_square_color(ChessSquare square);
size_t chess_square_from_algebraic(ChessSquare *square, const char *string);
size_t chess_square_to_algebraic(ChessSquare square, char *string, size_t string_size);
bool chess_square_is_attacked(const ChessPosition *position, ChessSquare square, ChessColor color);

#ifdef __cplusplus
}
#endif

#endif // CHESS_SQUARE_H_INCLUDED
