/**
 * @file chess/position.h
 * @brief Defines the chess position type and related utility functions for representing and manipulating chess positions.
 * @author Tarek Saeed
 * @date 2025-06-14
 */

#ifndef CHESS_POSITION_H_INCLUDED
#define CHESS_POSITION_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/castling_rights.h>
#include <chess/color.h>
#include <chess/piece.h>
#include <chess/position_counter.h>
#include <chess/square.h>

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
	#include <stdbool.h>
#endif
#include <stdint.h>

typedef struct ChessPosition {
	ChessPiece board[128];
	ChessColor side_to_move;
	ChessCastlingRights castling_rights;
	ChessSquare en_passant_square;
	unsigned int half_move_clock;
	unsigned int full_move_number;
	ChessSquare king_squares[CHESS_COLOR_BLACK + 1];
	ChessPositionCounter position_counter;
} ChessPosition;

void chess_position_debug(const ChessPosition *position);
bool chess_position_is_valid(const ChessPosition *position);
ChessPosition chess_position_new(void);
void chess_position_drop(ChessPosition *position);
ChessPiece chess_position_piece_at_square(const ChessPosition *position, ChessSquare square);
ChessColor chess_position_side_to_move(const ChessPosition *position);
size_t chess_position_from_fen(ChessPosition *position, const char *string);
size_t chess_position_to_fen(const ChessPosition *position, char *string, size_t string_size);
bool chess_position_is_check(const ChessPosition *position);
bool chess_position_is_checkmate(const ChessPosition *position);
bool chess_position_is_stalemate(const ChessPosition *position);
bool chess_position_is_fifty_move_rule(const ChessPosition *position);
bool chess_position_is_threefold_repetition(const ChessPosition *position);
bool chess_position_is_insufficient_material(const ChessPosition *position);
uint64_t chess_position_hash(const ChessPosition *position);

#ifdef __cplusplus
}
#endif

#endif // CHESS_POSITION_H_INCLUDED
