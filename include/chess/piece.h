/**
 * @file chess/piece.h
 * @brief Defines the chess piece type and related utility functions for representing and manipulating chess pieces.
 * @author Tarek Saeed
 * @date 2025-06-14
 */

#ifndef CHESS_PIECE_H_INCLUDED
#define CHESS_PIECE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/color.h>
#include <chess/macros.h>
#include <chess/piece_type.h>

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
	#include <stdbool.h>
#endif
#include <stdint.h>

/**
 * @enum ChessPieceType
 * @brief Represents the chess pieces.
 */
CHESS_ENUM(uint8_t, ChessPiece){
	CHESS_PIECE_WHITE_PAWN   = CHESS_COLOR_WHITE << 3U | CHESS_PIECE_TYPE_PAWN,   /**< White pawn piece */
	CHESS_PIECE_WHITE_KNIGHT = CHESS_COLOR_WHITE << 3U | CHESS_PIECE_TYPE_KNIGHT, /**< White knight piece */
	CHESS_PIECE_WHITE_BISHOP = CHESS_COLOR_WHITE << 3U | CHESS_PIECE_TYPE_BISHOP, /**< White bishop piece */
	CHESS_PIECE_WHITE_ROOK   = CHESS_COLOR_WHITE << 3U | CHESS_PIECE_TYPE_ROOK,   /**< White rook piece */
	CHESS_PIECE_WHITE_QUEEN  = CHESS_COLOR_WHITE << 3U | CHESS_PIECE_TYPE_QUEEN,  /**< White queen piece */
	CHESS_PIECE_WHITE_KING   = CHESS_COLOR_WHITE << 3U | CHESS_PIECE_TYPE_KING,   /**< White king piece */

	CHESS_PIECE_BLACK_PAWN   = CHESS_COLOR_BLACK << 3U | CHESS_PIECE_TYPE_PAWN,   /**< Black pawn piece */
	CHESS_PIECE_BLACK_KNIGHT = CHESS_COLOR_BLACK << 3U | CHESS_PIECE_TYPE_KNIGHT, /**< Black knight piece */
	CHESS_PIECE_BLACK_BISHOP = CHESS_COLOR_BLACK << 3U | CHESS_PIECE_TYPE_BISHOP, /**< Black bishop piece */
	CHESS_PIECE_BLACK_ROOK   = CHESS_COLOR_BLACK << 3U | CHESS_PIECE_TYPE_ROOK,   /**< Black rook piece */
	CHESS_PIECE_BLACK_QUEEN  = CHESS_COLOR_BLACK << 3U | CHESS_PIECE_TYPE_QUEEN,  /**< Black queen piece */
	CHESS_PIECE_BLACK_KING   = CHESS_COLOR_BLACK << 3U | CHESS_PIECE_TYPE_KING,   /**< Black king piece */

	CHESS_PIECE_NONE         = CHESS_COLOR_NONE << 3U | CHESS_PIECE_TYPE_NONE, /**< No piece */
};

/**
 * @brief Prints a debug representation of the given piece.
 * @param[in] piece The piece to print.
 */
void chess_piece_debug(ChessPiece piece);

/**
 * @brief Checks if the given piece is valid.
 * @param[in] piece The piece to check.
 * @return true if the piece is valid, false otherwise.
 */
bool chess_piece_is_valid(ChessPiece piece);

/**
 * @brief Creates a piece from a color and piece type.
 * @param[in] color The color of the piece.
 * @param[in] type The type of the piece.
 * @return The created piece.
 */
ChessPiece chess_piece_new(ChessColor color, ChessPieceType type);

/**
 * @brief Gets the color of the given piece.
 * @param[in] piece The piece.
 * @return The color of the piece.
 */
ChessColor chess_piece_color(ChessPiece piece);

/**
 * @brief Gets the type of the given piece.
 * @param[in] piece The piece.
 * @return The type of the piece.
 */
ChessPieceType chess_piece_type(ChessPiece piece);

/**
 * @brief Parses a piece from algebraic notation (e.g., "P", "N", ..., "K", "p", "n", ..., "k").
 * @param[out] piece Pointer to store the parsed piece.
 * @param[in] string The string containing the algebraic notation.
 * @return The number of characters read if successful, 0 otherwise.
 */
size_t chess_piece_from_algebraic(ChessPiece *piece, const char *string);

/**
 * @brief Converts a piece to algebraic notation (e.g., "P", "N", ..., "K", "p", "n", ..., "k").
 * @param[in] piece The piece to convert.
 * @param[out] string The buffer to store the algebraic notation.
 * @param[in] string_size The size of the output buffer.
 * @return The number of characters written.
 */
size_t chess_piece_to_algebraic(ChessPiece piece, char *string, size_t string_size);

#ifdef __cplusplus
}
#endif

#endif // CHESS_PIECE_H_INCLUDED
