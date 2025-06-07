#include <chess/position_counter.h>

#include <chess/position.h>

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CHESS_DEFINE_INTEGRAL_CONSTANT(size_t, CHESS_POSITION_COUNTER_INITIAL_SIZE, 64);
#define CHESS_POSITION_COUNTER_MAXIMUM_LOAD_FACTOR (0.75F)

bool chess_position_counter_is_valid(const ChessPositionCounter *counter) {
	assert(counter != CHESS_NULL);

	if (counter->entries == CHESS_NULL) {
		return counter->size == 0 && counter->count == 0;
	}

	if (counter->size == 0 || counter->count > counter->size) {
		return false;
	}

	size_t count = 0;
	for (size_t i = 0; i < counter->size; i++) {
		if (counter->entries[i].value != 0) {
			count++;
		}
	}

	return counter->count == count;
}
ChessPositionCounter chess_position_counter_new(void) {
	return (ChessPositionCounter){
		.entries = CHESS_NULL,
		.size    = 0,
		.count   = 0,
	};
}
void chess_position_counter_drop(ChessPositionCounter *counter) {
	assert(chess_position_counter_is_valid(counter));

	free(counter->entries);

	counter->entries = CHESS_NULL;
	counter->size    = 0;
	counter->count   = 0;
}
void chess_position_counter_clear(ChessPositionCounter *counter) {
	assert(chess_position_counter_is_valid(counter));

	memset(counter->entries, 0, counter->size * sizeof(counter->entries[0]));
	counter->count = 0;
}
static bool chess_position_counter_expand(ChessPositionCounter *counter) {
	assert(chess_position_counter_is_valid(counter));

	if (counter->size >= SIZE_MAX / sizeof(counter->entries[0])) {
		return false;
	}

	size_t size = 0;
	if (counter->size == 0) {
		size = CHESS_POSITION_COUNTER_INITIAL_SIZE;
	} else if (counter->size > SIZE_MAX / sizeof(counter->entries[0]) / 2) {
		size = SIZE_MAX / sizeof(*counter->entries);
	} else {
		size = counter->size * 2;
	}

	ChessPositionCounterEntry *entries = calloc(size, sizeof(*entries));
	if (entries == NULL) {
		return false;
	}

	if (counter->entries != CHESS_NULL) {
		for (size_t i = 0; i < counter->size; i++) {
			if (counter->entries[i].value != 0) {
				size_t probe = counter->entries[i].key % size;
				while (entries[probe].value != 0) {
					probe = (probe + 1) % size;
				}

				entries[probe] = counter->entries[i];
			}
		}

		free(counter->entries);
	}

	counter->entries = entries;
	counter->size    = size;

	return true;
}
unsigned int chess_position_counter_count(const ChessPositionCounter *counter, const ChessPosition *position) {
	assert(chess_position_counter_is_valid(counter));

	if (counter->entries == CHESS_NULL) {
		return 0;
	}

	uint64_t key = chess_position_hash(position);
	size_t probe = key % counter->size;
	while (counter->entries[probe].value != 0) {
		if (counter->entries[probe].key == key) {
			return counter->entries[probe].value;
		}
		probe = (probe + 1) % counter->size;
	}

	return 0;
}
bool chess_position_counter_increment(ChessPositionCounter *counter, const ChessPosition *position) {
	assert(chess_position_counter_is_valid(counter));

	if (counter->entries == CHESS_NULL || (float)counter->count / (float)counter->size > CHESS_POSITION_COUNTER_MAXIMUM_LOAD_FACTOR) {
		if (!chess_position_counter_expand(counter)) {
			return false;
		}
	}

	uint64_t key = chess_position_hash(position);
	size_t probe = key % counter->size;
	while (counter->entries[probe].value != 0) {
		if (counter->entries[probe].key == key) {
			counter->entries[probe].value++;
			return true;
		}
		probe = (probe + 1) % counter->size;
	}

	counter->entries[probe].key   = key;
	counter->entries[probe].value = 1;
	counter->count++;

	return true;
}
bool chess_position_counter_decrement(ChessPositionCounter *counter, const ChessPosition *position) {
	assert(chess_position_counter_is_valid(counter));

	if (counter->entries == CHESS_NULL) {
		return false;
	}

	uint64_t key   = chess_position_hash(position);
	size_t probe_1 = key % counter->size;
	while (counter->entries[probe_1].value != 0) {
		if (counter->entries[probe_1].key == key) {
			counter->entries[probe_1].value--;

			if (counter->entries[probe_1].value == 0) {
				counter->count--;

				for (size_t probe_2 = (probe_1 + 1) % counter->size; counter->entries[probe_2].value != 0; probe_2 = (probe_2 + 1) % counter->size) {
					size_t hash = counter->entries[probe_2].key % counter->size;
					if ((hash <= probe_2 && (hash < probe_1 && probe_1 <= probe_2)) || (hash > probe_2 && (probe_1 <= probe_2 || hash < probe_1))) {
						counter->entries[probe_1]       = counter->entries[probe_2];
						counter->entries[probe_2].value = 0;
						probe_1                         = probe_2;
					}
				}
			}

			return true;
		}
		probe_1 = (probe_1 + 1) % counter->size;
	}

	return false;
}
