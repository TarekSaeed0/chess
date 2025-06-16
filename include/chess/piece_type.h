/**
 * @file chess/piece_type.h
 * @brief Defines the chess piece type type and related utility functions for representing and manipulating chess piece types.
 * @author Tarek Saeed
 * @date 2025-06-14
 */

#ifndef CHESS_PIECE_TYPE_H_INCLUDED
#define CHESS_PIECE_TYPE_H_INCLUDED

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
 * @enum ChessPieceType
 * @brief Represents the types of chess pieces.
 */
CHESS_ENUM(uint8_t, ChessPieceType){
	CHESS_PIECE_TYPE_PAWN   = 0, /**< Pawn piece type */
	CHESS_PIECE_TYPE_KNIGHT = 1, /**< Knight piece type */
	CHESS_PIECE_TYPE_BISHOP = 2, /**< Bishop piece type */
	CHESS_PIECE_TYPE_ROOK   = 3, /**< Rook piece type */
	CHESS_PIECE_TYPE_QUEEN  = 4, /**< Queen piece type */
	CHESS_PIECE_TYPE_KING   = 5, /**< King piece type */

	CHESS_PIECE_TYPE_NONE   = 6, /**< No piece type */
};

/**
 * @brief Prints a debug representation of the given piece type.
 * @param[in] type The piece type to print.
 */
void chess_piece_type_debug(ChessPieceType type);

/**
 * @brief Checks if the given piece type is valid.
 * @param[in] type The piece type to check.
 * @return true if the piece type is valid, false otherwise.
 */
bool chess_piece_type_is_valid(ChessPieceType type);

/**
 * @brief Parses a piece type from algebraic notation (e.g., "P", "N", ..., "K").
 * @param[out] type Pointer to store the parsed piece type.
 * @param[in] string The string containing the algebraic notation.
 * @return The number of characters read if successful, 0 otherwise.
 */
size_t chess_piece_type_from_algebraic(ChessPieceType *type, const char *string);

/**
 * @brief Converts a piece type to algebraic notation (e.g., "P", "N", ..., "K").
 * @param[in] type The piece type to convert.
 * @param[out] string The buffer to store the algebraic notation.
 * @param[in] string_size The size of the output buffer.
 * @return The number of characters written.
 */
size_t chess_piece_type_to_algebraic(ChessPieceType type, char *string, size_t string_size);

#ifdef __cplusplus
}
#endif

#endif // CHESS_PIECE_TYPE_H_INCLUDED
