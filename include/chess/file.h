#ifndef CHESS_FILE_H_INCLUDED
#define CHESS_FILE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <chess/types.h>

void chess_file_debug(ChessFile file);
bool chess_file_is_valid(ChessFile file);
size_t chess_file_from_algebraic(ChessFile *file, const char *string);
size_t chess_file_to_algebraic(ChessFile file, char *string, size_t string_size);

#ifdef __cplusplus
}
#endif

#endif // CHESS_FILE_H_INCLUDED
