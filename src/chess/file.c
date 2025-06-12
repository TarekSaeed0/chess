#include <chess/file.h>

#include <assert.h>
#include <stdio.h>

void chess_file_debug(ChessFile file) {
	switch (file) {
		case CHESS_FILE_NONE:
			printf("CHESS_FILE_NONE");
			break;
		case CHESS_FILE_A:
			printf("CHESS_FILE_A");
			break;
		case CHESS_FILE_B:
			printf("CHESS_FILE_B");
			break;
		case CHESS_FILE_C:
			printf("CHESS_FILE_C");
			break;
		case CHESS_FILE_D:
			printf("CHESS_FILE_D");
			break;
		case CHESS_FILE_E:
			printf("CHESS_FILE_E");
			break;
		case CHESS_FILE_F:
			printf("CHESS_FILE_F");
			break;
		case CHESS_FILE_G:
			printf("CHESS_FILE_G");
			break;
		case CHESS_FILE_H:
			printf("CHESS_FILE_H");
			break;
		default:
			printf("(ChessFile)%d", file);
			break;
	}
}
bool chess_file_is_valid(ChessFile file) {
	return (file & 0x8U) == 0;
}
size_t chess_file_from_algebraic(ChessFile *file, const char *string) {
	assert(file != CHESS_NULL && string != CHESS_NULL);

	if (string[0] < 'a' || 'h' < string[0]) {
		return 0;
	}

	*file = (ChessFile)(CHESS_FILE_A + (string[0] - 'a'));

	assert(chess_file_is_valid(*file));

	return 1;
}
size_t chess_file_to_algebraic(ChessFile file, char *string, size_t string_size) {
	assert(chess_file_is_valid(file) && (string != CHESS_NULL || string_size == 0));

	if (string != CHESS_NULL && string_size >= 2) {
		string[0] = (char)('a' + (file - CHESS_FILE_A));
		string[1] = '\0';
	}

	return 1;
}
