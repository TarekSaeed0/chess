/**
 * @file chess/rank.h
 * @brief Defines the chess rank type and related utility functions for representing and manipulating chess ranks.
 * @author Tarek Saeed
 * @date 2025-06-14
 */

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

/**
 * @enum ChessRank
 * @brief Represents the ranks (rows) on a chessboard, from 1 to 8.
 */
CHESS_ENUM(int8_t, ChessRank){
	CHESS_RANK_1    = 0, /**< Rank 1 */
	CHESS_RANK_2    = 1, /**< Rank 2 */
	CHESS_RANK_3    = 2, /**< Rank 3 */
	CHESS_RANK_4    = 3, /**< Rank 4 */
	CHESS_RANK_5    = 4, /**< Rank 5 */
	CHESS_RANK_6    = 5, /**< Rank 6 */
	CHESS_RANK_7    = 6, /**< Rank 7 */
	CHESS_RANK_8    = 7, /**< Rank 8 */

	CHESS_RANK_NONE = 8, /**< No rank */
};

/**
 * @brief Prints a debug representation of the given rank.
 * @param[in] rank The rank to print.
 */
void chess_rank_debug(ChessRank rank);

/**
 * @brief Checks if the given rank is valid (1-8).
 * @param[in] rank The rank to check.
 * @return true if the rank is valid, false otherwise.
 */
bool chess_rank_is_valid(ChessRank rank);

/**
 * @brief Parses a rank from algebraic notation (e.g., "1", "2", ..., "8").
 * @param[out] rank Pointer to store the parsed rank.
 * @param[in] string The string containing the algebraic notation.
 * @return The number of characters read if successful, 0 otherwise.
 */
size_t chess_rank_from_algebraic(ChessRank *rank, const char *string);

/**
 * @brief Converts a rank to algebraic notation (e.g., "1", "2", ..., "8").
 * @param[in] rank The rank to convert.
 * @param[out] string The buffer to store the algebraic notation.
 * @param[in] string_size The size of the output buffer.
 * @return The number of characters written.
 */
size_t chess_rank_to_algebraic(ChessRank rank, char *string, size_t string_size);

#ifdef __cplusplus
}
#endif

#endif // CHESS_RANK_H_INCLUDED
