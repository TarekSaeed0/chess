/**
 * @file chess/move.h
 * @brief Defines the chess move type and related utility functions for representing and manipulating chess moves.
 * @author Tarek Saeed
 * @date 2025-06-14
 */

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

/**
 * @struct ChessMove
 * @brief Represents a move in a chess game.
 */
typedef struct ChessMove {
	ChessSquare from;                             /**< The source square of the move. */
	ChessSquare to;                               /**< The destination square of the move. */
	ChessPieceType promotion_type;                /**< The type of piece to promote to, or `CHESS_PIECE_TYPE_NONE`` if not a promotion. */
	ChessPiece captured_piece;                    /**< The piece captured by this move, or CHESS_PIECE_NONE if not a capture. */
	ChessCastlingRights previous_castling_rights; /**< The castling rights before the move was made. */
	ChessSquare previous_en_passant_square;       /**< The en passant square before the move was made. */
	unsigned int previous_half_move_clock;        /**< The half-move clock value before the move was made. */
} ChessMove;

/**
 * @brief Prints a debug representation of the given move.
 * @param[in] move The move to print.
 */
void chess_move_debug(ChessMove move);

/**
 * @brief Checks if the given move is valid.
 * @param[in] move The move to check.
 * @return true if the move is valid, false otherwise.
 */
bool chess_move_is_valid(ChessMove move);

/**
 * @brief Creates a new move for the given position.
 * @param[in] position Pointer to the position.
 * @param[in] from The source square.
 * @param[in] to The destination square.
 * @param[in] promotion_type The type of piece to promote to, or `CHESS_PIECE_TYPE_NONE` if not a promotion.
 * @return The created move.
 */
ChessMove chess_move_new(const ChessPosition *position, ChessSquare from, ChessSquare to, ChessPieceType promotion_type);

/**
 * @brief Parses a move from algebraic notation.
 * @param[in] position Pointer to the position, the move is for.
 * @param[out] move Pointer to store the parsed move.
 * @param[in] string The string containing the algebraic notation.
 * @return The number of characters read if successful, 0 otherwise.
 */
size_t chess_move_from_algebraic(const ChessPosition *position, ChessMove *move, const char *string);

/**
 * @brief Converts a move to algebraic notation.
 * @param[in] position Pointer to the position, the move is for.
 * @param[in] move The move to convert.
 * @param[out] string The buffer to store the algebraic notation.
 * @param[in] string_size The size of the output buffer.
 * @return The number of characters written.
 */
size_t chess_move_to_algebraic(const ChessPosition *position, ChessMove move, char *string, size_t string_size);

/**
 * @brief Checks if the given move is legal in the given position.
 * @param[in] position Pointer to the position, the move is for.
 * @param[in] move The move to check.
 * @return true if the move is legal, false otherwise.
 */
bool chess_move_is_legal(const ChessPosition *position, ChessMove move);

/**
 * @brief Checks if the given move is a promotion.
 * @param[in] position Pointer to the position, the move is for.
 * @param[in] move The move to check.
 * @return true if the move is a promotion, false otherwise.
 */
bool chess_move_is_promotion(const ChessPosition *position, ChessMove move);

/**
 * @brief Checks if the given move is an en passant capture.
 * @param[in] position Pointer to the position, the move is for.
 * @param[in] move The move to check.
 * @return true if the move is an en passant capture, false otherwise.
 */
bool chess_move_is_en_passant(const ChessPosition *position, ChessMove move);

/**
 * @brief Checks if the given move is a capture.
 * @param[in] position Pointer to the position, the move is for.
 * @param[in] move The move to check.
 * @return true if the move is a capture, false otherwise.
 */
bool chess_move_is_capture(const ChessPosition *position, ChessMove move);

/**
 * @brief Checks if the given move is kingside castling.
 * @param[in] position Pointer to the position, the move is for.
 * @param[in] move The move to check.
 * @return true if the move is kingside castling, false otherwise.
 */
bool chess_move_is_kingside_castling(const ChessPosition *position, ChessMove move);

/**
 * @brief Checks if the given move is queenside castling.
 * @param[in] position Pointer to the position, the move is for.
 * @param[in] move The move to check.
 * @return true if the move is queenside castling, false otherwise.
 */
bool chess_move_is_queenside_castling(const ChessPosition *position, ChessMove move);

/**
 * @brief Does the given move on the given position if legal.
 * @param[inout] position Pointer to the position to update.
 * @param[in] move The move to make.
 * @return true if the move was done, false otherwise.
 */
bool chess_move_do(ChessPosition *position, ChessMove move);

/**
 * @brief Undoes the given move on the given position if legal.
 * @param[inout] position Pointer to the position to update.
 * @param[in] move The move to undo.
 * @return true if the move was undone, false otherwise.
 */
bool chess_move_undo(ChessPosition *position, ChessMove move);

#ifdef __cplusplus
}
#endif

#endif // CHESS_MOVE_H_INCLUDED
