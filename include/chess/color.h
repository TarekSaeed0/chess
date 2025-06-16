/**
 * @file chess/color.h
 * @brief Defines the chess color type and related utility functions for representing and manipulating chess colors.
 * @author Tarek Saeed
 * @date 2025-06-14
 */

#ifndef CHESS_COLOR_H_INCLUDED
#define CHESS_COLOR_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/macros.h>

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
	#include <stdbool.h>
#endif
#include <stdint.h>

/**
 * @enum ChessColor
 * @brief Represents the color of a chess piece, square or player.
 */
CHESS_ENUM(uint8_t, ChessColor){
	CHESS_COLOR_WHITE = 0, /**< White color */
	CHESS_COLOR_BLACK = 1, /**< Black color */

	CHESS_COLOR_NONE  = 2, /**< No color */
};

/**
 * @brief Prints a debug representation of the given color.
 * @param color The color to print.
 */
void chess_color_debug(ChessColor color);

/**
 * @brief Checks if the given color is valid (white or black).
 * @param color The color to check.
 * @return true if the color is valid, false otherwise.
 */
bool chess_color_is_valid(ChessColor color);

/**
 * @brief Returns the opposite color (white <-> black).
 * @param color The color to invert.
 * @return The opposite color.
 */
ChessColor chess_color_opposite(ChessColor color);

#ifdef __cplusplus
}
#endif

#endif // CHESS_COLOR_H_INCLUDED
