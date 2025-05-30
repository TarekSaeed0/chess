#ifndef CHESS_H_INCLUDED
#define CHESS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/types.h>

#include <chess/color.h>
#include <chess/file.h>
#include <chess/move.h>
#include <chess/moves.h>
#include <chess/piece.h>
#include <chess/piece_type.h>
#include <chess/position.h>
#include <chess/rank.h>
#include <chess/square.h>

unsigned long chess_perft(unsigned int depth);

#ifdef __cplusplus
}
#endif

#endif // CHESS_H_INCLUDED
