/**
 * @file chess/position_counter.h
 * @brief Defines the chess position counter type and related functions for keeping count of position occurences.
 * @author Tarek Saeed
 * @date 2025-06-14
 */

#ifndef CHESS_POSITION_COUNTER_H_INCLUDED
#define CHESS_POSITION_COUNTER_H_INCLUDED

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
 * @brief Entry in the position counter hash table.
 *
 * Stores a position key and its associated count.
 */
typedef struct ChessPositionCounterEntry {
	uint64_t key;       /**< Hash key for the position. */
	unsigned int value; /**< Number of times the position has occurred. */
} ChessPositionCounterEntry;

/**
 * @brief Hash table for counting chess positions.
 *
 * Used to track the number of times each position has occurred,
 * typically for threefold repetition detection.
 */
typedef struct ChessPositionCounter {
	ChessPositionCounterEntry *entries; /**< Array of entries. */
	size_t size;                        /**< Allocated size of the table. */
	size_t count;                       /**< Number of entries in use. */
} ChessPositionCounter;

/**
 * @brief Checks if the position counter is valid.
 * @param[in] counter Pointer to the position counter.
 * @return true if the position counter is valid, false otherwise.
 */
bool chess_position_counter_is_valid(const ChessPositionCounter *counter);

/**
 * @brief Creates a new, empty position counter.
 * @return The created position counter.
 */
ChessPositionCounter chess_position_counter_new(void);

/**
 * @brief Releases resources held by the position counter.
 * @param[inout] counter Pointer to the position counter to drop.
 */
void chess_position_counter_drop(ChessPositionCounter *counter);

/**
 * @brief Clears all entries from the position counter.
 * @param[inout] counter Pointer to the position counter to clear.
 */
void chess_position_counter_clear(ChessPositionCounter *counter);

/**
 * @brief Gets the count for a given position.
 * @param[in] counter Pointer to the position counter.
 * @param[in] position Pointer to the position.
 * @return The number of times the position has occurred.
 */
unsigned int chess_position_counter_count(const ChessPositionCounter *counter, const ChessPosition *position);

/**
 * @brief Increments the count for a given position.
 * @param[inout] counter Pointer to the position counter.
 * @param[in] position Pointer to the position.
 * @return true on success, false on failure.
 */
bool chess_position_counter_increment(ChessPositionCounter *counter, const ChessPosition *position);

/**
 * @brief Decrements the count for a given position.
 * @param[inout] counter Pointer to the position counter.
 * @param[in] position Pointer to the position.
 * @return true on success, false on failure.
 */
bool chess_position_counter_decrement(ChessPositionCounter *counter, const ChessPosition *position);

#endif // CHESS_POSITION_COUNTER_H_INCLUDED
