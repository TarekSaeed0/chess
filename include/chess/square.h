/**
 * @file chess/square.h
 * @brief Defines the chess square type and related utility functions for representing and manipulating chess squares.
 * @author Tarek Saeed
 * @date 2025-06-14
 */

#ifndef CHESS_SQUARE_H_INCLUDED
#define CHESS_SQUARE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/color.h>
#include <chess/file.h>
#include <chess/macros.h>
#include <chess/rank.h>

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
	#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Forward declaration of ChessPosition.
 */
typedef struct ChessPosition ChessPosition;

/**
 * @enum ChessSquare
 * @brief Represents the squares on a chess board, from A1 to H8.
 */
CHESS_ENUM(uint8_t, ChessSquare){
	CHESS_SQUARE_A1   = CHESS_FILE_A | CHESS_RANK_1 << 4U, /**< Square A1 */
	CHESS_SQUARE_B1   = CHESS_FILE_B | CHESS_RANK_1 << 4U, /**< Square B1 */
	CHESS_SQUARE_C1   = CHESS_FILE_C | CHESS_RANK_1 << 4U, /**< Square C1 */
	CHESS_SQUARE_D1   = CHESS_FILE_D | CHESS_RANK_1 << 4U, /**< Square D1 */
	CHESS_SQUARE_E1   = CHESS_FILE_E | CHESS_RANK_1 << 4U, /**< Square E1 */
	CHESS_SQUARE_F1   = CHESS_FILE_F | CHESS_RANK_1 << 4U, /**< Square F1 */
	CHESS_SQUARE_G1   = CHESS_FILE_G | CHESS_RANK_1 << 4U, /**< Square G1 */
	CHESS_SQUARE_H1   = CHESS_FILE_H | CHESS_RANK_1 << 4U, /**< Square H1 */

	CHESS_SQUARE_A2   = CHESS_FILE_A | CHESS_RANK_2 << 4U, /**< Square A2 */
	CHESS_SQUARE_B2   = CHESS_FILE_B | CHESS_RANK_2 << 4U, /**< Square B2 */
	CHESS_SQUARE_C2   = CHESS_FILE_C | CHESS_RANK_2 << 4U, /**< Square C2 */
	CHESS_SQUARE_D2   = CHESS_FILE_D | CHESS_RANK_2 << 4U, /**< Square D2 */
	CHESS_SQUARE_E2   = CHESS_FILE_E | CHESS_RANK_2 << 4U, /**< Square E2 */
	CHESS_SQUARE_F2   = CHESS_FILE_F | CHESS_RANK_2 << 4U, /**< Square F2 */
	CHESS_SQUARE_G2   = CHESS_FILE_G | CHESS_RANK_2 << 4U, /**< Square G2 */
	CHESS_SQUARE_H2   = CHESS_FILE_H | CHESS_RANK_2 << 4U, /**< Square H2 */

	CHESS_SQUARE_A3   = CHESS_FILE_A | CHESS_RANK_3 << 4U, /**< Square A3 */
	CHESS_SQUARE_B3   = CHESS_FILE_B | CHESS_RANK_3 << 4U, /**< Square B3 */
	CHESS_SQUARE_C3   = CHESS_FILE_C | CHESS_RANK_3 << 4U, /**< Square C3 */
	CHESS_SQUARE_D3   = CHESS_FILE_D | CHESS_RANK_3 << 4U, /**< Square D3 */
	CHESS_SQUARE_E3   = CHESS_FILE_E | CHESS_RANK_3 << 4U, /**< Square E3 */
	CHESS_SQUARE_F3   = CHESS_FILE_F | CHESS_RANK_3 << 4U, /**< Square F3 */
	CHESS_SQUARE_G3   = CHESS_FILE_G | CHESS_RANK_3 << 4U, /**< Square G3 */
	CHESS_SQUARE_H3   = CHESS_FILE_H | CHESS_RANK_3 << 4U, /**< Square H3 */

	CHESS_SQUARE_A4   = CHESS_FILE_A | CHESS_RANK_4 << 4U, /**< Square A4 */
	CHESS_SQUARE_B4   = CHESS_FILE_B | CHESS_RANK_4 << 4U, /**< Square B4 */
	CHESS_SQUARE_C4   = CHESS_FILE_C | CHESS_RANK_4 << 4U, /**< Square C4 */
	CHESS_SQUARE_D4   = CHESS_FILE_D | CHESS_RANK_4 << 4U, /**< Square D4 */
	CHESS_SQUARE_E4   = CHESS_FILE_E | CHESS_RANK_4 << 4U, /**< Square E4 */
	CHESS_SQUARE_F4   = CHESS_FILE_F | CHESS_RANK_4 << 4U, /**< Square F4 */
	CHESS_SQUARE_G4   = CHESS_FILE_G | CHESS_RANK_4 << 4U, /**< Square G4 */
	CHESS_SQUARE_H4   = CHESS_FILE_H | CHESS_RANK_4 << 4U, /**< Square H4 */

	CHESS_SQUARE_A5   = CHESS_FILE_A | CHESS_RANK_5 << 4U, /**< Square A5 */
	CHESS_SQUARE_B5   = CHESS_FILE_B | CHESS_RANK_5 << 4U, /**< Square B5 */
	CHESS_SQUARE_C5   = CHESS_FILE_C | CHESS_RANK_5 << 4U, /**< Square C5 */
	CHESS_SQUARE_D5   = CHESS_FILE_D | CHESS_RANK_5 << 4U, /**< Square D5 */
	CHESS_SQUARE_E5   = CHESS_FILE_E | CHESS_RANK_5 << 4U, /**< Square E5 */
	CHESS_SQUARE_F5   = CHESS_FILE_F | CHESS_RANK_5 << 4U, /**< Square F5 */
	CHESS_SQUARE_G5   = CHESS_FILE_G | CHESS_RANK_5 << 4U, /**< Square G5 */
	CHESS_SQUARE_H5   = CHESS_FILE_H | CHESS_RANK_5 << 4U, /**< Square H5 */

	CHESS_SQUARE_A6   = CHESS_FILE_A | CHESS_RANK_6 << 4U, /**< Square A6 */
	CHESS_SQUARE_B6   = CHESS_FILE_B | CHESS_RANK_6 << 4U, /**< Square B6 */
	CHESS_SQUARE_C6   = CHESS_FILE_C | CHESS_RANK_6 << 4U, /**< Square C6 */
	CHESS_SQUARE_D6   = CHESS_FILE_D | CHESS_RANK_6 << 4U, /**< Square D6 */
	CHESS_SQUARE_E6   = CHESS_FILE_E | CHESS_RANK_6 << 4U, /**< Square E6 */
	CHESS_SQUARE_F6   = CHESS_FILE_F | CHESS_RANK_6 << 4U, /**< Square F6 */
	CHESS_SQUARE_G6   = CHESS_FILE_G | CHESS_RANK_6 << 4U, /**< Square G6 */
	CHESS_SQUARE_H6   = CHESS_FILE_H | CHESS_RANK_6 << 4U, /**< Square H6 */

	CHESS_SQUARE_A7   = CHESS_FILE_A | CHESS_RANK_7 << 4U, /**< Square A7 */
	CHESS_SQUARE_B7   = CHESS_FILE_B | CHESS_RANK_7 << 4U, /**< Square B7 */
	CHESS_SQUARE_C7   = CHESS_FILE_C | CHESS_RANK_7 << 4U, /**< Square C7 */
	CHESS_SQUARE_D7   = CHESS_FILE_D | CHESS_RANK_7 << 4U, /**< Square D7 */
	CHESS_SQUARE_E7   = CHESS_FILE_E | CHESS_RANK_7 << 4U, /**< Square E7 */
	CHESS_SQUARE_F7   = CHESS_FILE_F | CHESS_RANK_7 << 4U, /**< Square F7 */
	CHESS_SQUARE_G7   = CHESS_FILE_G | CHESS_RANK_7 << 4U, /**< Square G7 */
	CHESS_SQUARE_H7   = CHESS_FILE_H | CHESS_RANK_7 << 4U, /**< Square H7 */

	CHESS_SQUARE_A8   = CHESS_FILE_A | CHESS_RANK_8 << 4U, /**< Square A8 */
	CHESS_SQUARE_B8   = CHESS_FILE_B | CHESS_RANK_8 << 4U, /**< Square B8 */
	CHESS_SQUARE_C8   = CHESS_FILE_C | CHESS_RANK_8 << 4U, /**< Square C8 */
	CHESS_SQUARE_D8   = CHESS_FILE_D | CHESS_RANK_8 << 4U, /**< Square D8 */
	CHESS_SQUARE_E8   = CHESS_FILE_E | CHESS_RANK_8 << 4U, /**< Square E8 */
	CHESS_SQUARE_F8   = CHESS_FILE_F | CHESS_RANK_8 << 4U, /**< Square F8 */
	CHESS_SQUARE_G8   = CHESS_FILE_G | CHESS_RANK_8 << 4U, /**< Square G8 */
	CHESS_SQUARE_H8   = CHESS_FILE_H | CHESS_RANK_8 << 4U, /**< Square H8 */

	CHESS_SQUARE_NONE = CHESS_FILE_NONE | CHESS_RANK_NONE << 4U, /**< None square */
};

/**
 * @brief Prints a debug representation of the given square.
 * @param[in] square The square to print.
 */
void chess_square_debug(ChessSquare square);

/**
 * @brief Checks if the given square is valid (A1-H8).
 * @param[in] square The square to check.
 * @return true if the square is valid, false otherwise.
 */
bool chess_square_is_valid(ChessSquare square);

/**
 * @brief Creates a new square from a file and rank.
 * @param[in] file The file of the square.
 * @param[in] rank The rank of the square.
 * @return The created square.
 */
ChessSquare chess_square_new(ChessFile file, ChessRank rank);

/**
 * @brief Gets the file of the given square.
 * @param square The square.
 * @return The file of the square.
 */
ChessFile chess_square_file(ChessSquare square);

/**
 * @brief Gets the rank of the given square.
 * @param square The square.
 * @return The rank of the square.
 */
ChessRank chess_square_rank(ChessSquare square);

/**
 * @brief Gets the color of the given square.
 * @param square The square.
 * @return The color of the square.
 */
ChessColor chess_square_color(ChessSquare square);

/**
 * @brief Parses a square from algebraic notation (e.g., "a1", "b1", ..., "h8").
 * @param[out] square Pointer to store the parsed square.
 * @param[in] string The string containing the algebraic notation.
 * @return The number of characters read if successful, 0 otherwise.
 */
size_t chess_square_from_algebraic(ChessSquare *square, const char *string);

/**
 * @brief Converts a square to algebraic notation (e.g., "a1", "b1", ..., "h8").
 * @param[in] square The square to convert.
 * @param[out] string The buffer to store the algebraic notation.
 * @param[in] string_size The size of the output buffer.
 * @return The number of characters written.
 */
size_t chess_square_to_algebraic(ChessSquare square, char *string, size_t string_size);

/**
 * @brief Checks if the given square is attacked by a given color in a position.
 * @param[in] position The chess position.
 * @param[in] square The square to check.
 * @param[in] color The color to check for attacks.
 * @return true if the square is attacked, false otherwise.
 */
bool chess_square_is_attacked(const ChessPosition *position, ChessSquare square, ChessColor color);

#ifdef __cplusplus
}
#endif

#endif // CHESS_SQUARE_H_INCLUDED
