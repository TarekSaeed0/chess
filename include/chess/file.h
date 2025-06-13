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

CHESS_ENUM(int8_t, ChessFile) {
	CHESS_FILE_A    = 0,
	CHESS_FILE_B    = 1,
	CHESS_FILE_C    = 2,
	CHESS_FILE_D    = 3,
	CHESS_FILE_E    = 4,
	CHESS_FILE_F    = 5,
	CHESS_FILE_G    = 6,
	CHESS_FILE_H    = 7,

	CHESS_FILE_NONE = 8,
};

void chess_file_debug(ChessFile file);
bool chess_file_is_valid(ChessFile file);
size_t chess_file_from_algebraic(ChessFile *file, const char *string);
size_t chess_file_to_algebraic(ChessFile file, char *string, size_t string_size);

#ifdef __cplusplus
}
#endif

#endif // CHESS_FILE_H_INCLUDED
