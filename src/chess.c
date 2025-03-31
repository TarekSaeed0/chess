#include <chess.h>

static inline bool chess_square_is_off_the_board(enum chess_square square) {
	return (square & 0x88U) != 0U;
}

static void chess_get_attackers_from(
	const struct chess *chess,
	struct chess_squares *attackers,
	enum chess_square to,
	struct chess_get_attackers_filter filter
) {
	assert(
		chess != nullptr && attackers != nullptr && filter.flags.from &&
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

	static const uint8_t can_attack[256] = {
		// clang-format off
			6, 0, 0, 0, 0, 0, 0,18, 0, 0, 0, 0, 0, 0, 6, 0,
			0, 6, 0, 0, 0, 0, 0,18, 0, 0, 0, 0, 0, 6, 0, 0,
			0, 0, 6, 0, 0, 0, 0,18, 0, 0, 0, 0, 6, 0, 0, 0,
			0, 0, 0, 6, 0, 0, 0,18, 0, 0, 0, 6, 0, 0, 0, 0,
			0, 0, 0, 0, 6, 0, 0,18, 0, 0, 6, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 6, 8,18, 8, 6, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 8,39,19,39, 8, 0, 0, 0, 0, 0, 0,
		 18,18,18,18,18,18,19, 0,19,18,18,18,18,18,18, 0,
			0, 0, 0, 0, 0, 8,39,19,39, 8, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 6, 8,18, 8, 6, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 6, 0, 0,18, 0, 0, 6, 0, 0, 0, 0, 0,
			0, 0, 0, 6, 0, 0, 0,18, 0, 0, 0, 6, 0, 0, 0, 0,
			0, 0, 6, 0, 0, 0, 0,18, 0, 0, 0, 0, 6, 0, 0, 0,
			0, 6, 0, 0, 0, 0, 0,18, 0, 0, 0, 0, 0, 6, 0, 0,
			6, 0, 0, 0, 0, 0, 0,18, 0, 0, 0, 0, 0, 0, 6, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		// clang-format on
	};

	int8_t difference = (int8_t)(from - to);
	uint8_t index = (uint8_t)(difference + 0x77);
	enum chess_piece_type type = chess_piece_get_type(piece);
	if (can_attack[index] & (1U << type)) {
		switch (type) {
			case chess_piece_type_pawn: {
				if ((difference > 0 && color == chess_piece_color_white) ||
					(difference < 0 && color == chess_piece_color_black)) {
					attackers->squares[attackers->count++] = from;
				}
			} break;
			case chess_piece_type_king:
			case chess_piece_type_knight: {
				attackers->squares[attackers->count++] = from;
			} break;
			case chess_piece_type_queen:
			case chess_piece_type_bishop:
			case chess_piece_type_rook: {
				static const int8_t direction[] = {
					// clang-format off
					 17,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0, 15,  0,
						0, 17,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0, 15,  0,  0,
						0,  0, 17,  0,  0,  0,  0, 16,  0,  0,  0,  0, 15,  0,  0,  0,
						0,  0,  0, 17,  0,  0,  0, 16,  0,  0,  0, 15,  0,  0,  0,  0,
						0,  0,  0,  0, 17,  0,  0, 16,  0,  0, 15,  0,  0,  0,  0,  0,
						0,  0,  0,  0,  0, 17,  0, 16,  0, 15,  0,  0,  0,  0,  0,  0,
						0,  0,  0,  0,  0,  0, 17, 16, 15,  0,  0,  0,  0,  0,  0,  0,
						1,  1,  1,  1,  1,  1,  1,  0, -1, -1,  -1,-1, -1, -1, -1,  0,
						0,  0,  0,  0,  0,  0,-15,-16,-17,  0,  0,  0,  0,  0,  0,  0,
						0,  0,  0,  0,  0,-15,  0,-16,  0,-17,  0,  0,  0,  0,  0,  0,
						0,  0,  0,  0,-15,  0,  0,-16,  0,  0,-17,  0,  0,  0,  0,  0,
						0,  0,  0,-15,  0,  0,  0,-16,  0,  0,  0,-17,  0,  0,  0,  0,
						0,  0,-15,  0,  0,  0,  0,-16,  0,  0,  0,  0,-17,  0,  0,  0,
						0,-15,  0,  0,  0,  0,  0,-16,  0,  0,  0,  0,  0,-17,  0,  0,
					-15,  0,  0,  0,  0,  0,  0,-16,  0,  0,  0,  0,  0,  0,-17,  0,
						0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
					// clang-format on
				};

				enum chess_square current = (enum chess_square)(from + direction[index]);
				while (current != to) {
					if (chess->board[current] != chess_piece_none) {
						return;
					}
					current = (enum chess_square)(current + direction[index]);
				}

				attackers->squares[attackers->count++] = from;
			} break;
		}
	}
}
struct chess_squares chess_get_attackers(
	const struct chess *chess,
	enum chess_square to,
	struct chess_get_attackers_filter filter
) {
	assert(
		chess != nullptr && !chess_square_is_off_the_board(to) &&
		(!filter.flags.from || !chess_square_is_off_the_board(filter.from))
	);

	struct chess_squares attackers = { .count = 0 };

	if (filter.flags.from) {
		chess_get_attackers_from(chess, &attackers, to, filter);
	} else {
		filter.flags.from = true;
		for (enum chess_square from = chess_square_a8; from <= chess_square_h1; from++) {
			if (from & 0x88U) {
				from += 7;
				continue;
			}

			filter.from = from;
			chess_get_attackers_from(chess, &attackers, to, filter);
		}
	}

	return attackers;
}

bool chess_is_king_attacked(const struct chess *chess, enum chess_piece_color color) {
	assert(chess != nullptr);

	enum chess_piece king_piece = chess_piece_new(chess_piece_type_king, color);

	enum chess_square king_square = 0;
	for (enum chess_square square = chess_square_a8; square <= chess_square_h1; square++) {
		if (square & 0x88U) {
			square += 7;
			continue;
		}

		if (chess->board[square] == king_piece) {
			king_square = square;
			break;
		}
	}

	return chess_get_attackers(
			   chess,
			   king_square,
			   (struct chess_get_attackers_filter){
				   .flags.color = true,
				   .color = chess_piece_color_opposite(chess->turn),
			   }
		   )
			   .count != 0;
}

static inline void chess_get_legal_moves_from_to(
	const struct chess *chess,
	struct chess_moves *moves,
	struct chess_get_moves_filter filter,
	enum chess_square to
) {
	enum chess_square from = filter.from;

	if (filter.flags.to && to != filter.to) {
		return;
	}

	struct chess temporary = *chess;
	temporary.board[to] = temporary.board[from];
	temporary.board[from] = chess_piece_none;
	if (chess_is_king_attacked(&temporary, chess->turn)) {
		return;
	}

	moves->moves[moves->count++] = (struct chess_move){
		.from = from,
		.to = to,
	};
}
static void chess_get_legal_moves_from(
	const struct chess *chess,
	struct chess_moves *moves,
	struct chess_get_moves_filter filter
) {
	assert(
		chess != nullptr && moves != nullptr && filter.flags.from &&
		!chess_square_is_off_the_board(filter.from) &&
		(!filter.flags.to || !chess_square_is_off_the_board(filter.to))
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
				chess_get_legal_moves_from_to(chess, moves, filter, to);

				if (chess_square_get_rank(from) == pawn_ranks[color]) {
					to = (enum chess_square)(from + pawn_offsets[color][1]);
					if (!chess_square_is_off_the_board(to) &&
						chess->board[to] == chess_piece_none) {
						chess_get_legal_moves_from_to(chess, moves, filter, to);
					}
				}

				for (size_t i = 2; i < 4; i++) {
					to = (enum chess_square)(from + pawn_offsets[color][i]);
					if (!chess_square_is_off_the_board(to) &&
						chess->board[to] != chess_piece_none &&
						chess_piece_get_color(chess->board[to]) != color) {
						chess_get_legal_moves_from_to(chess, moves, filter, to);
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

					chess_get_legal_moves_from_to(chess, moves, filter, to);
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
						chess_get_legal_moves_from_to(chess, moves, filter, to);
					} else {
						if (chess_piece_get_color(chess->board[to]) != color) {
							chess_get_legal_moves_from_to(chess, moves, filter, to);
						}
						break;
					}
				}
			}
		}
	}
}
struct chess_moves chess_get_legal_moves(
	const struct chess *chess,
	struct chess_get_moves_filter filter
) {
	assert(
		chess != nullptr && (!filter.flags.from || !chess_square_is_off_the_board(filter.from)) &&
		(!filter.flags.to || !chess_square_is_off_the_board(filter.to))
	);

	struct chess_moves moves = { .count = 0 };

	if (filter.flags.from) {
		chess_get_legal_moves_from(chess, &moves, filter);
	} else {
		filter.flags.from = true;
		for (enum chess_square from = chess_square_a8; from <= chess_square_h1; from++) {
			if (from & 0x88U) {
				from += 7;
				continue;
			}

			filter.from = from;
			chess_get_legal_moves_from(chess, &moves, filter);
		}
	}

	return moves;
}

bool chess_is_legal_move(const struct chess *chess, struct chess_move move) {
	assert(chess != nullptr);

	return chess_get_legal_moves(chess, (struct chess_get_moves_filter) {
    .flags = {
      .from = true,
      .to = true,
			.color = true,
    },
    .from = move.from,
    .to = move.to,
		.color = chess->turn,
  }).count != 0;
}

bool chess_move(struct chess *chess, struct chess_move move) {
	assert(chess != nullptr);

	if (!chess_is_legal_move(chess, move)) {
		return false;
	}

	chess->board[move.to] = chess->board[move.from];
	chess->board[move.from] = chess_piece_none;

	chess->turn = chess_piece_color_opposite(chess->turn);

	return true;
}

enum chess_status chess_get_status(const struct chess *chess) {
	if (chess_is_king_attacked(chess, chess->turn)) {
		if (chess_get_legal_moves(
				chess,
				(struct chess_get_moves_filter){
					.flags.color = true,
					.color = chess->turn,
				}
			)
				.count == 0) {
			return chess_status_checkmate;
		}
		return chess_status_check;
	}

	return chess_status_ongoing;
}
