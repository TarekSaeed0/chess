#ifndef CHESS_SQUARE_H_INCLUDED
#define CHESS_SQUARE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/types.h>

void chess_square_debug(ChessSquare square);
bool chess_square_is_valid(ChessSquare square);
ChessSquare chess_square_new(ChessFile file, ChessRank rank);
ChessFile chess_square_file(ChessSquare square);
ChessRank chess_square_rank(ChessSquare square);
ChessColor chess_square_color(ChessSquare square);
size_t chess_square_from_algebraic(ChessSquare *square, const char *string);
size_t chess_square_to_algebraic(ChessSquare square, char *string, size_t string_size);
bool chess_square_is_attacked(const ChessPosition *position, ChessSquare square, ChessColor color);

#ifdef __cplusplus
}
#endif

#endif // CHESS_SQUARE_H_INCLUDED
