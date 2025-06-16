/**
 * @file chess/file.h
 * @brief Defines the chess file type and related utility functions for representing and manipulating chess files.
 * @author Tarek Saeed
 * @date 2025-06-14
 */

#ifndef CHESS_FILE_H_INCLUDED
#define CHESS_FILE_H_INCLUDED

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
 * @enum ChessFile
 * @brief Represents the files (columns) on a chessboard, from A to H.
 */
CHESS_ENUM(int8_t, ChessFile){
	CHESS_FILE_A    = 0, /**< File A */
	CHESS_FILE_B    = 1, /**< File B */
	CHESS_FILE_C    = 2, /**< File C */
	CHESS_FILE_D    = 3, /**< File D */
	CHESS_FILE_E    = 4, /**< File E */
	CHESS_FILE_F    = 5, /**< File F */
	CHESS_FILE_G    = 6, /**< File G */
	CHESS_FILE_H    = 7, /**< File H */

	CHESS_FILE_NONE = 8, /**< No file */
};

/**
 * @brief Prints a debug representation of the given file.
 * @param[in] file The file to print.
 */
void chess_file_debug(ChessFile file);

/**
 * @brief Checks if the given file is valid (A-H).
 * @param[in] file The file to check.
 * @return true if the file is valid, false otherwise.
 */
bool chess_file_is_valid(ChessFile file);

/**
 * @brief Parses a file from algebraic notation (e.g., "a", "b", ... "h").
 * @param[out] file Pointer to store the parsed file.
 * @param[in] string The string containing the algebraic notation.
 * @return The number of characters read if successful, 0 otherwise.
 */
size_t chess_file_from_algebraic(ChessFile *file, const char *string);

/**
 * @brief Converts a file to algebraic notation (e.g., "a", "b", ... "h").
 * @param[in] file The file to convert.
 * @param[out] string The buffer to store the algebraic notation.
 * @param[in] string_size The size of the output buffer.
 * @return The number of characters written.
 */
size_t chess_file_to_algebraic(ChessFile file, char *string, size_t string_size);

#ifdef __cplusplus
}
#endif

#endif // CHESS_FILE_H_INCLUDED
