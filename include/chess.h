/**
 * @file chess.h
 * @brief Chess library using a 0x88-based chess board representation.
 *
 * @see https://en.wikipedia.org/wiki/0x88
 *
 * @author Tarek Saeed
 * @date 2025-03-23
 */

#ifndef CHESS_H
#define CHESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @enum chess_piece_type
 * @brief Represents the different types of chess pieces.
 */
enum chess_piece_type : uint8_t {
	chess_piece_type_king,	 ///< King piece
	chess_piece_type_queen,	 ///< Queen piece
	chess_piece_type_bishop, ///< Bishop piece
	chess_piece_type_knight, ///< Knight piece
	chess_piece_type_rook,	 ///< Rook piece
	chess_piece_type_pawn,	 ///< Pawn piece
};

/**
 * @enum chess_piece_color
 * @brief Represents the color of a chess piece.
 */
enum chess_piece_color : uint8_t {
	chess_piece_color_white, ///< White piece
	chess_piece_color_black, ///< White piece
};

/**
 * @brief Returns the opposite color of a given chess piece color.
 *
 * @param color The chess piece color.
 *
 * @return The opposite chess piece color.
 */
static inline enum chess_piece_color chess_piece_color_opposite(enum chess_piece_color color) {
	return (enum chess_piece_color)(color ^ 1U);
}

/**
 * @enum chess_piece
 * @brief Represents a chess piece including type and color.
 */
enum chess_piece : uint8_t {
	chess_piece_none,		  ///< No piece
	chess_piece_white_king,	  ///< White King
	chess_piece_black_king,	  ///< Black King
	chess_piece_white_queen,  ///< White Queen
	chess_piece_black_queen,  ///< Black Queen
	chess_piece_white_bishop, ///< White Bishop
	chess_piece_black_bishop, ///< Black Bishop
	chess_piece_white_knight, ///< White Knight
	chess_piece_black_knight, ///< Black Knight
	chess_piece_white_rook,	  ///< White Rook
	chess_piece_black_rook,	  ///< Black Rook
	chess_piece_white_pawn,	  ///< White Pawn
	chess_piece_black_pawn	  ///< Black Pawn
};

/**
 * @brief Creates a new chess piece from type and color.
 *
 * @param type The type of the chess piece.
 * @param color The color of the chess piece.
 *
 * @return The corresponding chess piece.
 */
static inline enum chess_piece chess_piece_new(
	enum chess_piece_type type,
	enum chess_piece_color color
) {
	return (enum chess_piece)(((uint8_t)(type << 1U) | color) + 1);
}

/**
 * @brief Retrieves the type of a given chess piece.
 *
 * @param piece The chess piece.
 *
 * @return The type of the chess piece.
 */
static inline enum chess_piece_type chess_piece_get_type(enum chess_piece piece) {
	assert(piece != chess_piece_none);

	return (enum chess_piece_type)((uint8_t)(piece - 1) >> 1U);
}

/**
 * @brief Retrieves the color of a given chess piece.
 *
 * @param piece The chess piece.
 *
 * @return The color of the chess piece.
 */
static inline enum chess_piece_color chess_piece_get_color(enum chess_piece piece) {
	assert(piece != chess_piece_none);

	return (enum chess_piece_color)((uint8_t)(piece - 1) & 1U);
}

/**
 * @enum chess_square
 * @brief Represents a chessboard square.
 */
enum chess_square : uint8_t {
	chess_square_a8 = 0x00,
	chess_square_b8 = 0x01,
	chess_square_c8 = 0x02,
	chess_square_d8 = 0x03,
	chess_square_e8 = 0x04,
	chess_square_f8 = 0x05,
	chess_square_g8 = 0x06,
	chess_square_h8 = 0x07,

	chess_square_a7 = 0x10,
	chess_square_b7 = 0x11,
	chess_square_c7 = 0x12,
	chess_square_d7 = 0x13,
	chess_square_e7 = 0x14,
	chess_square_f7 = 0x15,
	chess_square_g7 = 0x16,
	chess_square_h7 = 0x17,

	chess_square_a6 = 0x20,
	chess_square_b6 = 0x21,
	chess_square_c6 = 0x22,
	chess_square_d6 = 0x23,
	chess_square_e6 = 0x24,
	chess_square_f6 = 0x25,
	chess_square_g6 = 0x26,
	chess_square_h6 = 0x27,

	chess_square_a5 = 0x30,
	chess_square_b5 = 0x31,
	chess_square_c5 = 0x32,
	chess_square_d5 = 0x33,
	chess_square_e5 = 0x34,
	chess_square_f5 = 0x35,
	chess_square_g5 = 0x36,
	chess_square_h5 = 0x37,

	chess_square_a4 = 0x40,
	chess_square_b4 = 0x41,
	chess_square_c4 = 0x42,
	chess_square_d4 = 0x43,
	chess_square_e4 = 0x44,
	chess_square_f4 = 0x45,
	chess_square_g4 = 0x46,
	chess_square_h4 = 0x47,

	chess_square_a3 = 0x50,
	chess_square_b3 = 0x51,
	chess_square_c3 = 0x52,
	chess_square_d3 = 0x53,
	chess_square_e3 = 0x54,
	chess_square_f3 = 0x55,
	chess_square_g3 = 0x56,
	chess_square_h3 = 0x57,

	chess_square_a2 = 0x60,
	chess_square_b2 = 0x61,
	chess_square_c2 = 0x62,
	chess_square_d2 = 0x63,
	chess_square_e2 = 0x64,
	chess_square_f2 = 0x65,
	chess_square_g2 = 0x66,
	chess_square_h2 = 0x67,

	chess_square_a1 = 0x70,
	chess_square_b1 = 0x71,
	chess_square_c1 = 0x72,
	chess_square_d1 = 0x73,
	chess_square_e1 = 0x74,
	chess_square_f1 = 0x75,
	chess_square_g1 = 0x76,
	chess_square_h1 = 0x77,
};

/**
 * @brief Creates a new chess square from file and rank.
 *
 * @param file The file (column) of the square (0-7, corresponding to a-h).
 * @param rank The rank (row) of the square (0-7, corresponding to 8-1).
 *
 * @return The corresponding chess square.
 */
static inline enum chess_square chess_square_new(uint8_t file, uint8_t rank) {
	assert(file < 8 && rank < 8);

	return (enum chess_square)(file | (uint8_t)(rank << 4U));
}

/**
 * @brief Retrieves the file (column) from a given chess square.
 *
 * @param square The chess square.
 *
 * @return The file (0-7, corresponding to a-h).
 */
static inline uint8_t chess_square_get_file(enum chess_square square) {
	return square & 0x7U;
}

/**
 * @brief Retrieves the rank (row) from a given chess square.
 *
 * @param square The chess square.
 *
 * @return The rank (0-7, corresponding to 8-1).
 */
static inline uint8_t chess_square_get_rank(enum chess_square square) {
	return square >> 4U;
}

/**
 * @struct chess
 * @brief Represents a chess game.
 */
struct chess {
	enum chess_piece board[128]; ///< The chess board.
	enum chess_piece_color turn; ///< The color of the current player.
};
/**
 * @brief Creates a new chess game.
 *
 * Initializes a chess game with the standard starting position and white to play.
 *
 * @return The new chess game.
 */
static inline struct chess chess_new(void) {
	return (struct chess){
		.board = { 
			[chess_square_a8] = chess_piece_black_rook,
			[chess_square_b8] = chess_piece_black_knight,
			[chess_square_c8] = chess_piece_black_bishop,
			[chess_square_d8] = chess_piece_black_queen,
			[chess_square_e8] = chess_piece_black_king,
			[chess_square_f8] = chess_piece_black_bishop,
			[chess_square_g8] = chess_piece_black_knight,
			[chess_square_h8] = chess_piece_black_rook,

			[chess_square_a7] = chess_piece_black_pawn,
			[chess_square_b7] = chess_piece_black_pawn,
			[chess_square_c7] = chess_piece_black_pawn,
			[chess_square_d7] = chess_piece_black_pawn,
			[chess_square_e7] = chess_piece_black_pawn,
			[chess_square_f7] = chess_piece_black_pawn,
			[chess_square_g7] = chess_piece_black_pawn,
			[chess_square_h7] = chess_piece_black_pawn,

			[chess_square_a2] = chess_piece_white_pawn,
			[chess_square_b2] = chess_piece_white_pawn,
			[chess_square_c2] = chess_piece_white_pawn,
			[chess_square_d2] = chess_piece_white_pawn,
			[chess_square_e2] = chess_piece_white_pawn,
			[chess_square_f2] = chess_piece_white_pawn,
			[chess_square_g2] = chess_piece_white_pawn,
			[chess_square_h2] = chess_piece_white_pawn,

			[chess_square_a1] = chess_piece_white_rook,
			[chess_square_b1] = chess_piece_white_knight,
			[chess_square_c1] = chess_piece_white_bishop,
			[chess_square_d1] = chess_piece_white_queen,
			[chess_square_e1] = chess_piece_white_king,
			[chess_square_f1] = chess_piece_white_bishop,
			[chess_square_g1] = chess_piece_white_knight,
			[chess_square_h1] = chess_piece_white_rook,
		},
		.turn = chess_piece_color_white,
	};
}

/**
 * @struct chess_move
 * @brief Represents a chess move.
 */
struct chess_move {
	enum chess_square from; ///< The starting square of the move.
	enum chess_square to;	///< The ending square of the move.
};

/**
 * @var CHESS_MOVES_MAXIMUM_COUNT
 * @brief The maximum number of moves that can be generated. which must be at least 218.
 *
 * @see https://www.stmintz.com/ccc/index.php?id=424966
 */
constexpr size_t CHESS_MOVES_MAXIMUM_COUNT = 256;

/**
 * @struct chess_moves
 * @brief Represents a list of chess moves.
 */
struct chess_moves {
	struct chess_move moves[CHESS_MOVES_MAXIMUM_COUNT]; ///< The list of moves.
	size_t count;										///< The number of moves in the list.
};

/**
 * @struct chess_get_moves_filter
 * @brief Represents a filter for chess move generation.
 */
struct chess_get_moves_filter {
	struct {
		bool from : 1;	///< Flag indicating whether to filter moves by starting square.
		bool to : 1;	///< Flag indicating whether to filter moves by starting and ending square.
		bool color : 1; ///< Flag indicating whether to filter moves by chess piece color.
	} flags;			///< Flags indicating which fields to filter by.
	enum chess_piece_color color; ///< The color of the chess piece to move.
	enum chess_square from;		  ///< The starting square of the move.
	enum chess_square to;		  ///< The ending square of the move.
};

/**
 * @brief Generates a list of legal moves for a given chess position.
 *
 * @param chess The chess position to generate moves for.
 * @param filter A filter to apply to the generated moves. The filter can be used to
 *               filter moves by starting square, ending square, or color.
 *
 * @return A list of legal moves for the given chess position.
 *
 * @memberof chess
 */
struct chess_moves chess_get_moves(const struct chess *chess, struct chess_get_moves_filter filter);

/**
 * @brief Makes a move on the chess board.
 *
 * @param chess The chess position to make a move on.
 * @param move  The move to make.
 *
 * @return True if the move was successful, false otherwise. The move will fail if it
 *         is not a legal move according to the rules of chess.
 *
 * @memberof chess
 */
bool chess_move(struct chess *chess, struct chess_move move);

/**
 * @enum chess_status
 * Represents the status of a chess game.
 */
enum chess_status {
	chess_status_ongoing,
	chess_status_check,
};

enum chess_status chess_get_status(const struct chess *chess);

#ifdef __cplusplus
}
#endif

#endif // CHESS_H
