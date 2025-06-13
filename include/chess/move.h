#ifndef CHESS_MOVE_H_INCLUDED
#define CHESS_MOVE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/castling_rights.h>
#include <chess/piece.h>
#include <chess/piece_type.h>
#include <chess/position.h>
#include <chess/square.h>

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
	#include <stdbool.h>
#endif
#include <stddef.h>

typedef struct ChessMove {
	ChessSquare from;
	ChessSquare to;
	ChessPieceType promotion_type;
	ChessPiece captured_piece;
	ChessCastlingRights previous_castling_rights;
	ChessSquare previous_en_passant_square;
	unsigned int previous_half_move_clock;
} ChessMove;

void chess_move_debug(ChessMove move);
bool chess_move_is_valid(ChessMove move);
ChessMove chess_move_new(const ChessPosition *position, ChessSquare from, ChessSquare to, ChessPieceType promotion_type);
size_t chess_move_from_algebraic(const ChessPosition *position, ChessMove *move, const char *string);
size_t chess_move_to_algebraic(const ChessPosition *position, ChessMove move, char *string, size_t string_size);
bool chess_move_is_legal(const ChessPosition *position, ChessMove move);
bool chess_move_is_promotion(const ChessPosition *position, ChessMove move);
bool chess_move_is_en_passant(const ChessPosition *position, ChessMove move);
bool chess_move_is_capture(const ChessPosition *position, ChessMove move);
bool chess_move_is_kingside_castling(const ChessPosition *position, ChessMove move);
bool chess_move_is_queenside_castling(const ChessPosition *position, ChessMove move);
bool chess_move_do(ChessPosition *position, ChessMove move);
bool chess_move_undo(ChessPosition *position, ChessMove move);

#ifdef __cplusplus
}
#endif

#endif // CHESS_MOVE_H_INCLUDED
