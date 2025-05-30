#include <chess/moves.h>

#include <chess/color.h>
#include <chess/file.h>
#include <chess/move.h>
#include <chess/move_private.h>
#include <chess/offset.h>
#include <chess/piece.h>
#include <chess/piece_type.h>
#include <chess/position.h>
#include <chess/position_private.h>
#include <chess/rank.h>
#include <chess/square.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void chess_moves_add(ChessMoves *moves, const ChessPosition *position, ChessMove move) {
	assert(moves != CHESS_NULL && chess_position_is_valid(position) && chess_move_is_valid(move));

	ChessPosition position_after_move = *position;
	chess_move_do_unchecked(&position_after_move, move);
	if (!chess_position_is_king_attacked(&position_after_move, position->side_to_move)) {
		assert(moves->count < CHESS_MOVES_MAXIMUM_COUNT);
		moves->moves[moves->count++] = move;
	}
}
static void chess_moves_generate_pawn_promotions(
    ChessMoves *moves,
    const ChessPosition *position,
    ChessSquare from,
    ChessSquare to
) {
	assert(moves != CHESS_NULL && chess_position_is_valid(position) && chess_square_is_valid(from) && chess_square_is_valid(to));

	static CHESS_CONSTEXPR ChessPieceType promotion_types[] = {
		CHESS_PIECE_TYPE_KNIGHT,
		CHESS_PIECE_TYPE_BISHOP,
		CHESS_PIECE_TYPE_ROOK,
		CHESS_PIECE_TYPE_QUEEN,
	};
	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(promotion_types); i++) {
		chess_moves_add(
		    moves,
		    position,
		    (ChessMove){
		        .from           = from,
		        .to             = to,
		        .promotion_type = promotion_types[i],
		    }
		);
	}
}
static void chess_moves_generate_pawn(
    ChessMoves *moves,
    const ChessPosition *position,
    ChessSquare from
) {
	assert(moves != CHESS_NULL && chess_position_is_valid(position) && chess_square_is_valid(from));

	chess_offset direction   = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_OFFSET_NORTH : CHESS_OFFSET_SOUTH;

	ChessRank promotion_rank = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_RANK_8 : CHESS_RANK_1;

	ChessSquare to           = (ChessSquare)(from + direction);
	if (chess_square_is_valid(to) && position->board[to] == CHESS_PIECE_NONE) {
		if (chess_square_rank(to) == promotion_rank) {
			chess_moves_generate_pawn_promotions(moves, position, from, to);
		} else {
			chess_moves_add(
			    moves,
			    position,
			    (ChessMove){
			        .from           = from,
			        .to             = to,
			        .promotion_type = CHESS_PIECE_TYPE_NONE,
			    }
			);

			ChessRank start_rank = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_RANK_2 : CHESS_RANK_7;
			if (chess_square_rank(from) == start_rank) {
				to = (ChessSquare)(from + 2 * direction);
				if (position->board[to] == CHESS_PIECE_NONE) {
					chess_moves_add(
					    moves,
					    position,
					    (ChessMove){
					        .from           = from,
					        .to             = to,
					        .promotion_type = CHESS_PIECE_TYPE_NONE,
					    }
					);
				}
			}
		}
	}

	static CHESS_CONSTEXPR chess_offset offsets[] = {
		CHESS_OFFSET_EAST,
		CHESS_OFFSET_WEST,
	};
	for (size_t i = 0; i < CHESS_ARRAY_LENGTH(offsets); i++) {
		to = (ChessSquare)(from + direction + offsets[i]);
		if (to == position->en_passant_square) {
			chess_moves_add(
			    moves,
			    position,
			    (ChessMove){
			        .from           = from,
			        .to             = to,
			        .promotion_type = CHESS_PIECE_TYPE_NONE,
			    }
			);
		} else if (chess_square_is_valid(to) &&
		           chess_piece_color(position->board[to]) == chess_color_opposite(position->side_to_move)) {
			if (chess_square_rank(to) == promotion_rank) {
				chess_moves_generate_pawn_promotions(moves, position, from, to);
			} else {
				chess_moves_add(
				    moves,
				    position,
				    (ChessMove){
				        .from           = from,
				        .to             = to,
				        .promotion_type = CHESS_PIECE_TYPE_NONE,
				    }
				);
			}
		}
	}
}
static void chess_moves_generate_directions(
    ChessMoves *moves,
    const ChessPosition *position,
    ChessSquare from,
    const chess_offset *directions,
    size_t direction_count
) {
	assert(moves != CHESS_NULL && chess_position_is_valid(position) && (directions != CHESS_NULL || direction_count == 0));

	for (size_t i = 0; i < direction_count; i++) {
		chess_offset direction = directions[i];
		ChessSquare to         = from;
		while (true) {
			to += direction;
			if (!chess_square_is_valid(to)) {
				break;
			}

			if (position->board[to] == CHESS_PIECE_NONE) {
				chess_moves_add(
				    moves,
				    position,
				    (ChessMove){
				        .from           = from,
				        .to             = to,
				        .promotion_type = CHESS_PIECE_TYPE_NONE,
				    }
				);
			} else {
				if (chess_piece_color(position->board[to]) != position->side_to_move) {
					chess_moves_add(
					    moves,
					    position,
					    (ChessMove){
					        .from           = from,
					        .to             = to,
					        .promotion_type = CHESS_PIECE_TYPE_NONE,
					    }
					);
				}
				break;
			}
		}
	}
}
static void chess_moves_generate_offsets(
    ChessMoves *moves,
    const ChessPosition *position,
    ChessSquare from,
    const chess_offset *offsets,
    size_t offset_count
) {
	assert(moves != CHESS_NULL && chess_position_is_valid(position) && (offsets != CHESS_NULL || offset_count == 0));

	for (size_t i = 0; i < offset_count; i++) {
		ChessSquare to = (ChessSquare)(from + offsets[i]);
		if (chess_square_is_valid(to) && chess_piece_color(position->board[to]) != position->side_to_move) {
			chess_moves_add(
			    moves,
			    position,
			    (ChessMove){
			        .from           = from,
			        .to             = to,
			        .promotion_type = CHESS_PIECE_TYPE_NONE,
			    }
			);
		}
	}
}
static void chess_moves_generate_castlings(ChessMoves *moves, const ChessPosition *position) {
	assert(moves != CHESS_NULL && chess_position_is_valid(position));

	ChessSquare from                              = position->side_to_move == CHESS_COLOR_WHITE ? CHESS_SQUARE_E1 : CHESS_SQUARE_E8;
	ChessCastlingRights king_side_castling_right  = position->side_to_move == CHESS_COLOR_WHITE
	                                                    ? CHESS_CASTLING_RIGHTS_WHITE_KINGSIDE
	                                                    : CHESS_CASTLING_RIGHTS_BLACK_KINGSIDE;
	ChessCastlingRights queen_side_castling_right = position->side_to_move == CHESS_COLOR_WHITE
	                                                    ? CHESS_CASTLING_RIGHTS_WHITE_QUEENSIDE
	                                                    : CHESS_CASTLING_RIGHTS_BLACK_QUEENSIDE;

	ChessColor other_side                         = chess_color_opposite(position->side_to_move);

	if (position->castling_rights & king_side_castling_right) {
		if (position->board[from + CHESS_OFFSET_EAST] == CHESS_PIECE_NONE &&
		    position->board[from + 2 * CHESS_OFFSET_EAST] == CHESS_PIECE_NONE &&
		    !chess_square_is_attacked(position, from, other_side) &&
		    !chess_square_is_attacked(position, (ChessSquare)(from + CHESS_OFFSET_EAST), other_side) &&
		    !chess_square_is_attacked(position, (ChessSquare)(from + 2 * CHESS_OFFSET_EAST), other_side)) {
			chess_moves_add(
			    moves,
			    position,
			    (ChessMove){
			        .from           = from,
			        .to             = (ChessSquare)(from + 2 * CHESS_OFFSET_EAST),
			        .promotion_type = CHESS_PIECE_TYPE_NONE,
			    }
			);
		}
	}
	if (position->castling_rights & queen_side_castling_right) {
		if (position->board[from + CHESS_OFFSET_WEST] == CHESS_PIECE_NONE &&
		    position->board[from + 2 * CHESS_OFFSET_WEST] == CHESS_PIECE_NONE &&
		    position->board[from + 3 * CHESS_OFFSET_WEST] == CHESS_PIECE_NONE &&
		    !chess_square_is_attacked(position, from, other_side) &&
		    !chess_square_is_attacked(position, (ChessSquare)(from + CHESS_OFFSET_WEST), other_side) &&
		    !chess_square_is_attacked(position, (ChessSquare)(from + 2 * CHESS_OFFSET_WEST), other_side)) {
			chess_moves_add(
			    moves,
			    position,
			    (ChessMove){
			        .from           = from,
			        .to             = (ChessSquare)(from + 2 * CHESS_OFFSET_WEST),
			        .promotion_type = CHESS_PIECE_TYPE_NONE,
			    }
			);
		}
	}
}
ChessMoves chess_moves_generate(const ChessPosition *position) {
	assert(chess_position_is_valid(position));

	ChessMoves moves = { 0 };

	for (ChessRank rank = CHESS_RANK_8; rank >= CHESS_RANK_1; rank--) {
		for (ChessFile file = CHESS_FILE_A; file <= CHESS_FILE_H; file++) {
			ChessSquare from = chess_square_new(file, rank);

			ChessPiece piece = position->board[from];

			ChessColor color = chess_piece_color(piece);
			if (color != position->side_to_move) {
				continue;
			}

			ChessPieceType type = chess_piece_type(piece);
			switch (type) {
				case CHESS_PIECE_TYPE_PAWN: {
					chess_moves_generate_pawn(&moves, position, from);
				} break;
				case CHESS_PIECE_TYPE_KNIGHT: {
					static CHESS_CONSTEXPR chess_offset offsets[] = {
						2 * CHESS_OFFSET_NORTH + CHESS_OFFSET_EAST,
						2 * CHESS_OFFSET_NORTH + CHESS_OFFSET_WEST,
						2 * CHESS_OFFSET_EAST + CHESS_OFFSET_NORTH,
						2 * CHESS_OFFSET_EAST + CHESS_OFFSET_SOUTH,
						2 * CHESS_OFFSET_SOUTH + CHESS_OFFSET_EAST,
						2 * CHESS_OFFSET_SOUTH + CHESS_OFFSET_WEST,
						2 * CHESS_OFFSET_WEST + CHESS_OFFSET_NORTH,
						2 * CHESS_OFFSET_WEST + CHESS_OFFSET_SOUTH,
					};
					chess_moves_generate_offsets(&moves, position, from, offsets, CHESS_ARRAY_LENGTH(offsets));
				} break;
				case CHESS_PIECE_TYPE_BISHOP: {
					static CHESS_CONSTEXPR chess_offset directions[] = {
						CHESS_OFFSET_NORTH_EAST,
						CHESS_OFFSET_SOUTH_EAST,
						CHESS_OFFSET_SOUTH_WEST,
						CHESS_OFFSET_NORTH_WEST,
					};
					chess_moves_generate_directions(&moves, position, from, directions, CHESS_ARRAY_LENGTH(directions));
				} break;
				case CHESS_PIECE_TYPE_ROOK: {
					static CHESS_CONSTEXPR chess_offset directions[] = {
						CHESS_OFFSET_NORTH,
						CHESS_OFFSET_EAST,
						CHESS_OFFSET_SOUTH,
						CHESS_OFFSET_WEST,
					};
					chess_moves_generate_directions(&moves, position, from, directions, CHESS_ARRAY_LENGTH(directions));
				} break;
				case CHESS_PIECE_TYPE_QUEEN: {
					static CHESS_CONSTEXPR chess_offset directions[] = {
						CHESS_OFFSET_NORTH,
						CHESS_OFFSET_EAST,
						CHESS_OFFSET_SOUTH,
						CHESS_OFFSET_WEST,
						CHESS_OFFSET_NORTH_EAST,
						CHESS_OFFSET_SOUTH_EAST,
						CHESS_OFFSET_SOUTH_WEST,
						CHESS_OFFSET_NORTH_WEST,
					};
					chess_moves_generate_directions(&moves, position, from, directions, CHESS_ARRAY_LENGTH(directions));
				} break;
				case CHESS_PIECE_TYPE_KING: {
					static CHESS_CONSTEXPR chess_offset offsets[] = {
						CHESS_OFFSET_NORTH,
						CHESS_OFFSET_EAST,
						CHESS_OFFSET_SOUTH,
						CHESS_OFFSET_WEST,
						CHESS_OFFSET_NORTH_EAST,
						CHESS_OFFSET_SOUTH_EAST,
						CHESS_OFFSET_SOUTH_WEST,
						CHESS_OFFSET_NORTH_WEST,
					};
					chess_moves_generate_offsets(&moves, position, from, offsets, CHESS_ARRAY_LENGTH(offsets));
				} break;
				default: assert(false);
			}
		}
	}

	chess_moves_generate_castlings(&moves, position);

	return moves;
}
