#ifndef CHESS_MOVES_H_INCLUDED
#define CHESS_MOVES_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/macros.h>
#include <chess/move.h>

#include <stddef.h>

CHESS_DEFINE_INTEGRAL_CONSTANT(size_t, CHESS_MOVES_MAXIMUM_COUNT, 256);

typedef struct ChessMoves {
	struct ChessMove moves[CHESS_MOVES_MAXIMUM_COUNT];
	size_t count;
} ChessMoves;

ChessMoves chess_moves_generate_from(const ChessPosition *position, ChessSquare from);
ChessMoves chess_moves_generate(const ChessPosition *position);

#ifdef __cplusplus
}
#endif

#endif // CHESS_MOVES_H_INCLUDED
