/**
 * @file chess/castling_rights.h
 * @brief Defines the chess castling rights type and related utility functions for representing and manipulating chess castling rights.
 * @author Tarek Saeed
 * @date 2025-06-14
 */

#ifndef CHESS_CASTLING_RIGHTS_H_INCLUDED
#define CHESS_CASTLING_RIGHTS_H_INCLUDED

#include <chess/macros.h>

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
	#include <stdbool.h>
#endif
#include <stdint.h>

/**
 * @enum ChessCastlingRights
 * @brief Represents the castling rights in a chess game.
 */
CHESS_ENUM(uint8_t, ChessCastlingRights){
	CHESS_CASTLING_RIGHTS_NONE            = 0U, /**< No castling rights. */

	CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE  = 1U << 0U, /**< White can castle kingside. */
	CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE = 1U << 1U, /**< White can castle queenside. */
	CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE  = 1U << 2U, /**< Black can castle kingside. */
	CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE = 1U << 3U, /**< Black can castle queenside. */

	CHESS_CASTLING_RIGHTS_WHITE           = CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE | CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE, /**< White can castle on either side. */
	CHESS_CASTLING_RIGHTS_BLACK           = CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE | CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE, /**< Black can castle on either side. */

	CHESS_CASTLING_RIGHTS_ALL             = CHESS_CASTLING_RIGHTS_WHITE | CHESS_CASTLING_RIGHTS_BLACK, /**< White and black can both castle on either side. */
};

/**
 * @brief Prints a debug representation of the given castling rights.
 * @param[in] rights The castling rights to print.
 */
void chess_castling_rights_debug(ChessCastlingRights rights);

/**
 * @brief Checks if the given castling rights are valid.
 * @param rights The castling rights to check.
 * @return[in] true if the castling rights are valid, false otherwise.
 */
bool chess_castling_rights_is_valid(ChessCastlingRights rights);

#endif // CHESS_CASTLING_RIGHTS_H_INCLUDED
