#ifndef CHESS_MACROS_PRIVATE_H_INCLUDED
#define CHESS_MACROS_PRIVATE_H_INCLUDED

#include <stdio.h>

#define WRITE(function, ...)                                      \
	do {                                                            \
		size_t _written = function(__VA_ARGS__, string, string_size); \
		if (_written < string_size) {                                 \
			if (string != CHESS_NULL) {                                 \
				string += _written;                                       \
			}                                                           \
			string_size -= _written;                                    \
		} else {                                                      \
			string_size = 0;                                            \
		}                                                             \
		total_written += _written;                                    \
	} while (false)
#define WRITE_FORMATTED(...)                                   \
	do {                                                         \
		int _written = snprintf(string, string_size, __VA_ARGS__); \
		assert(_written >= 0);                                     \
		if ((size_t)_written < string_size) {                      \
			if (string != CHESS_NULL) {                              \
				string += (size_t)_written;                            \
			}                                                        \
			string_size -= (size_t)_written;                         \
		} else {                                                   \
			string_size = 0;                                         \
		}                                                          \
		total_written += (size_t)_written;                         \
	} while (false)

#define READ(function, ...)                                    \
	do {                                                         \
		size_t _read = function(__VA_ARGS__, &string[total_read]); \
		if (_read == 0) {                                          \
			return 0;                                                \
		}                                                          \
		total_read += _read;                                       \
	} while (false)

#endif // CHESS_MACROS_PRIVATE_H_INCLUDED
