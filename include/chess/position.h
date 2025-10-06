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

/**
 * @struct ChessPosition
 * @brief Represents the position in a chess game.
 */
typedef struct ChessPosition {
	ChessPiece board[128];                                                    /**< Array representing the pieces on each square (0x88 board). */
	ChessSquare pieces[CHESS_COLOR_BLACK + 1][CHESS_PIECE_TYPE_KING + 1][12]; /**< Array storing the squares of each piece type for both colors. */
	uint8_t piece_counts[CHESS_COLOR_BLACK + 1][CHESS_PIECE_TYPE_KING + 1];   /**< Array storing the count of each piece type for both colors. */
	ChessColor side_to_move;                                                  /**< The color of the side to move next. */
	ChessCastlingRights castling_rights;                                      /**< The current castling rights for both sides. */
	ChessSquare en_passant_square;                                            /**< The square over which a pawn has just passed while moving two squares, or `CHESS_SQUARE_NONE` if not available. */
	unsigned int half_move_clock;                                             /**< The number of halfmoves since the last capture or pawn advance, used for the fifty-move rule. */
	unsigned int full_move_number;                                            /**< The number of the full moves. It starts at 1 and is incremented after Black's move. */
	ChessPositionCounter position_counter;                                    /**< Counter for position repetitions (for threefold repetition rule). */
} ChessPosition;

/**
 * @brief Prints a debug representation of the given position.
 * @param[in] position Pointer to the position to print.
 */
void chess_position_debug(const ChessPosition *position);

/**
 * @brief Checks if the given position is valid.
 * @param[in] position Pointer to the position to check.
 * @return true if the position is valid, false otherwise.
 */
bool chess_position_is_valid(const ChessPosition *position);

/**
 * @brief Creates a new position initialized to the standard starting position.
 * @return The created position.
 */
ChessPosition chess_position_new(void);

/**
 * @brief Releases resources held by the given position.
 * @param[inout] position Pointer to the position to drop.
 */
void chess_position_drop(ChessPosition *position);

/**
 * @brief Gets the piece at the given square in the given position.
 * @param[in] position Pointer to the position.
 * @param[in] square The square.
 * @return The piece at the square.
 */
ChessPiece chess_position_piece_at_square(const ChessPosition *position, ChessSquare square);

/**
 * @brief Gets the side to move in the given position.
 * @param[in] position Pointer to the position.
 * @return The color of the side to move.
 */
ChessColor chess_position_side_to_move(const ChessPosition *position);

/**
 * @brief Parses a position from FEN notation (e.g., "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1").
 * @param[out] position Pointer to store the parsed position.
 * @param[in] string The string containing the FEN notation.
 * @return The number of characters read if successful, 0 otherwise.
 */
size_t chess_position_from_fen(ChessPosition *position, const char *string);

/**
 * @brief Converts a position to FEN notation (e.g., "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1").
 * @param[in] position Pointer to the position to convert.
 * @param[out] string The buffer to store the FEN notation.
 * @param[in] string_size The size of the output buffer.
 * @return The number of characters written.
 */
size_t chess_position_to_fen(const ChessPosition *position, char *string, size_t string_size);

/**
 * @brief Checks if the current side to move is in check.
 * @param[in] position Pointer to the position.
 * @return true if the side to move is in check, false otherwise.
 */
bool chess_position_is_check(const ChessPosition *position);

/**
 * @brief Checks if the current side to move is in checkmate.
 * @param[in] position Pointer to the position.
 * @return true if the side to move is in checkmate, false otherwise.
 */
bool chess_position_is_checkmate(const ChessPosition *position);

/**
 * @brief Checks if the current position is a stalemate.
 * @param[in] position Pointer to the position.
 * @return true if the position is a stalemate, false otherwise.
 */
bool chess_position_is_stalemate(const ChessPosition *position);

/**
 * @brief Checks if the fifty-move rule applies to the current position.
 * @param[in] position Pointer to the position.
 * @return true if the fifty-move rule applies, false otherwise.
 */
bool chess_position_is_fifty_move_rule(const ChessPosition *position);

/**
 * @brief Checks if the current position has occurred three or more times (threefold repetition).
 * @param[in] position Pointer to the position.
 * @return true if the position has occurred three or more times, false otherwise.
 */
bool chess_position_is_threefold_repetition(const ChessPosition *position);

/**
 * @brief Checks if there is insufficient material to checkmate.
 * @param[in] position Pointer to the position.
 * @return true if there is insufficient material, false otherwise.
 */
bool chess_position_is_insufficient_material(const ChessPosition *position);

/**
 * @brief Computes a hash value for the given position.
 * @param[in] position Pointer to the position.
 * @return The hash value of the position.
 */
uint64_t chess_position_hash(const ChessPosition *position);

/**
 * @brief Evaluates the given position.
 * @param[in] position Pointer to the position.
 * @return The value of the position (positive for White's advantage, negative for Black's advantage).
 */
double chess_position_evaluate(const ChessPosition *position);

#ifdef __cplusplus
}
#endif

#endif // CHESS_POSITION_H_INCLUDED
