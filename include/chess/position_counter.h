#ifndef CHESS_POSITION_COUNTER_H_INCLUDED
#define CHESS_POSITION_COUNTER_H_INCLUDED

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
	#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

typedef struct ChessPosition ChessPosition;

typedef struct ChessPositionCounterEntry {
	uint64_t key;
	unsigned int value;
} ChessPositionCounterEntry;

typedef struct ChessPositionCounter {
	ChessPositionCounterEntry *entries;
	size_t size;
	size_t count;
} ChessPositionCounter;

bool chess_position_counter_is_valid(const ChessPositionCounter *counter);
ChessPositionCounter chess_position_counter_new(void);
void chess_position_counter_drop(ChessPositionCounter *counter);
void chess_position_counter_clear(ChessPositionCounter *counter);
unsigned int chess_position_counter_count(const ChessPositionCounter *counter, const ChessPosition *position);
bool chess_position_counter_increment(ChessPositionCounter *counter, const ChessPosition *position);
bool chess_position_counter_decrement(ChessPositionCounter *counter, const ChessPosition *position);

#endif // CHESS_POSITION_COUNTER_H_INCLUDED
