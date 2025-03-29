#include <chess.h>

static inline bool chess_square_is_off_the_board(enum chess_square square) {
	return (square & 0x88U) != 0U;
}

bool chess_moves_contain(const struct chess_moves *moves, struct chess_move move) {
	assert(moves != nullptr);

	for (size_t i = 0; i < moves->count; i++) {
		if (moves->moves[i].from == move.from && moves->moves[i].to == move.to) {
			return true;
		}
	}
	return false;
}

void chess_get_moves_from(
	const struct chess *chess,
	struct chess_moves *moves,
	struct chess_get_moves_filter filter
) {
	assert(
		chess != nullptr && moves != nullptr && filter.flags.from &&
		!chess_square_is_off_the_board(filter.from)
	);

	enum chess_square from = filter.from;

	enum chess_piece piece = chess->board[from];
	if (piece == chess_piece_none) {
		return;
	}

	enum chess_piece_color color = chess_piece_get_color(piece);
	if (filter.flags.color && color != filter.color) {
		return;
	}

	if (filter.flags.to) {
		static const uint8_t flags[256] = {
			// clang-format off
			6, 0, 0, 0, 0, 0, 0,18, 0, 0, 0, 0, 0, 0, 6, 0,
			0, 6, 0, 0, 0, 0, 0,18, 0, 0, 0, 0, 0, 6, 0, 0,
			0, 0, 6, 0, 0, 0, 0,18, 0, 0, 0, 0, 6, 0, 0, 0,
			0, 0, 0, 6, 0, 0, 0,18, 0, 0, 0, 6, 0, 0, 0, 0,
			0, 0, 0, 0, 6, 0, 0,18, 0, 0, 6, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 6, 8,18, 8, 6, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 8, 7,19, 7, 8, 0, 0, 0, 0, 0, 0,
		 18,18,18,18,18,18,19, 0,19,18,18,18,18,18,18, 0,
			0, 0, 0, 0, 0, 8, 7,19, 7, 8, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 6, 8,18, 8, 6, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 6, 0, 0,18, 0, 0, 6, 0, 0, 0, 0, 0,
			0, 0, 0, 6, 0, 0, 0,18, 0, 0, 0, 6, 0, 0, 0, 0,
			0, 0, 6, 0, 0, 0, 0,18, 0, 0, 0, 0, 6, 0, 0, 0,
			0, 6, 0, 0, 0, 0, 0,18, 0, 0, 0, 0, 0, 6, 0, 0,
			6, 0, 0, 0, 0, 0, 0,18, 0, 0, 0, 0, 0, 0, 6, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			// clang-format on
		};

		enum chess_square to = filter.to;

		uint8_t index = to - from + 0x77;
	}

	enum chess_piece_type type = chess_piece_get_type(piece);
	switch (type) {
		case chess_piece_type_pawn: {
			static const int8_t pawn_offsets[2][4] = {
				[chess_piece_color_white] = { -16, -32, -17, -15 },
				[chess_piece_color_black] = { 16, 32, 17, 15 },
			};
			static const uint8_t pawn_ranks[] = {
				[chess_piece_color_white] = 6,
				[chess_piece_color_black] = 1,
			};

			enum chess_square to = (enum chess_square)(from + pawn_offsets[color][0]);
			if (!chess_square_is_off_the_board(to) && chess->board[to] == chess_piece_none) {
				if (!filter.flags.to || to == filter.to) {
					moves->moves[moves->count++] = (struct chess_move){
						.from = from,
						.to = to,
					};
				}

				if (chess_square_get_rank(from) == pawn_ranks[color]) {
					to = (enum chess_square)(from + pawn_offsets[color][1]);
					if (!chess_square_is_off_the_board(to) &&
						chess->board[to] == chess_piece_none) {
						if (!filter.flags.to || to == filter.to) {
							moves->moves[moves->count++] = (struct chess_move){
								.from = from,
								.to = to,
							};
						}
					}
				}

				for (size_t i = 2; i < 4; i++) {
					to = (enum chess_square)(from + pawn_offsets[color][i]);
					if (!chess_square_is_off_the_board(to) &&
						chess->board[to] != chess_piece_none &&
						chess_piece_get_color(chess->board[to]) != color) {
						if (!filter.flags.to || to == filter.to) {
							moves->moves[moves->count++] = (struct chess_move){
								.from = from,
								.to = to,
							};
						}
					}
				}
			}
		} break;
		case chess_piece_type_king:
		case chess_piece_type_knight: {
			static const int8_t king_offsets[] = { -17, -16, -15, 1, 17, 16, 15, -1 };
			static const int8_t knight_offsets[] = { -33, -31, -18, -14, 18, 31, 33, 14 };
			static const struct {
				const int8_t *offsets;
				size_t count;
			} piece_offsets[] = {
				[chess_piece_type_king] = { 
					.offsets = king_offsets,
					.count = sizeof(king_offsets) / sizeof(king_offsets[0]),
				},
				[chess_piece_type_knight] = {
					.offsets = knight_offsets,
					.count = sizeof(knight_offsets) / sizeof(knight_offsets[0]),
				},
			};

			for (size_t i = 0; i < piece_offsets[type].count; i++) {
				enum chess_square to = (enum chess_square)(from + piece_offsets[type].offsets[i]);
				if (!chess_square_is_off_the_board(to) &&
					(chess->board[to] == chess_piece_none ||
					 chess_piece_get_color(chess->board[to]) != color)) {

					if (!filter.flags.to || to == filter.to) {
						moves->moves[moves->count++] = (struct chess_move){
							.from = from,
							.to = to,
						};
					}
				}
			}
		} break;
		case chess_piece_type_queen:
		case chess_piece_type_bishop:
		case chess_piece_type_rook: {
			static const int8_t queen_offsets[] = { -17, -16, -15, 1, 17, 16, 15, -1 };
			static const int8_t bishop_offsets[] = { -17, -15, 17, 15 };
			static const int8_t rook_offsets[] = { -16, 1, 16, -1 };
			static const struct {
				const int8_t *offsets;
				size_t count;
			} piece_offsets[] = {
				[chess_piece_type_queen] = {
					.offsets = queen_offsets,
					.count = sizeof(queen_offsets) / sizeof(queen_offsets[0]),
				},
				[chess_piece_type_bishop] = {
					.offsets = bishop_offsets,
					.count = sizeof(bishop_offsets) / sizeof(bishop_offsets[0]),
				},
				[chess_piece_type_rook] = {
					.offsets = rook_offsets,
					.count = sizeof(rook_offsets) / sizeof(rook_offsets[0]),
				},
			};

			for (size_t i = 0; i < piece_offsets[type].count; i++) {
				int8_t offset = piece_offsets[type].offsets[i];

				enum chess_square to = from;
				while (true) {
					to += offset;
					if (chess_square_is_off_the_board(to)) {
						break;
					}

					if (chess->board[to] == chess_piece_none) {
						if (!filter.flags.to || to == filter.to) {
							moves->moves[moves->count++] = (struct chess_move){
								.from = from,
								.to = to,
							};
						}
					} else {
						if (chess_piece_get_color(chess->board[to]) != color) {
							if (!filter.flags.to || to == filter.to) {
								moves->moves[moves->count++] = (struct chess_move){
									.from = from,
									.to = to,
								};
							}
						}
						break;
					}
				}
			}
		}
	}
}
struct chess_moves chess_get_moves(
	const struct chess *chess,
	struct chess_get_moves_filter filter
) {
	assert(
		chess != nullptr && (!filter.flags.from || !chess_square_is_off_the_board(filter.from)) &&
		(!filter.flags.to || !chess_square_is_off_the_board(filter.to))
	);

	struct chess_moves moves = { .count = 0 };

	if (filter.flags.from) {
		chess_get_moves_from(chess, &moves, filter);
	} else {
		filter.flags.from = true;
		for (enum chess_square from = chess_square_a8; from <= chess_square_h1; from++) {
			if (from & 0x88U) {
				from += 7;
				continue;
			}

			filter.from = from;
			chess_get_moves_from(chess, &moves, filter);
		}
	}

	return moves;
}
bool chess_move(struct chess *chess, struct chess_move move) {
	assert(chess != nullptr);

	struct chess_moves moves = chess_get_moves(chess, (struct chess_get_moves_filter) {
    .flags = {
      .from = true,
      .to = true,
			.color = true,
    },
    .from = move.from,
    .to = move.to,
		.color = chess->turn,
  });

	if (moves.count == 0) {
		return false;
	}

	chess->board[move.to] = chess->board[move.from];
	chess->board[move.from] = chess_piece_none;

	chess->turn = chess_piece_color_opposite(chess->turn);

	return true;
}

enum chess_status chess_get_status(const struct chess *chess) {
	enum chess_square king_square = 0;
	for (enum chess_square square = chess_square_a8; square <= chess_square_h1; square++) {
		if (square & 0x88U) {
			square += 7;
			continue;
		}

		if (chess->board[square] == chess_piece_new(chess_piece_type_king, chess->turn)) {
			king_square = square;
			break;
		}
	}

	struct chess_moves moves = chess_get_moves(
		chess,
		(struct chess_get_moves_filter) {
			.flags = {
				.color = true,
				.to = true,
			},
			.color = chess_piece_color_opposite(chess->turn),
			.to = king_square,
		}
	);

	if (moves.count != 0) {
		return chess_status_check;
	}

	return chess_status_ongoing;
}
