#include <chess/rank.h>

#include <assert.h>
#include <stdio.h>

void chess_rank_debug(ChessRank rank) {
	switch (rank) {
		case CHESS_RANK_NONE:
			printf("CHESS_RANK_NONE");
			break;
		case CHESS_RANK_1:
			printf("CHESS_RANK_1");
			break;
		case CHESS_RANK_2:
			printf("CHESS_RANK_2");
			break;
		case CHESS_RANK_3:
			printf("CHESS_RANK_3");
			break;
		case CHESS_RANK_4:
			printf("CHESS_RANK_4");
			break;
		case CHESS_RANK_5:
			printf("CHESS_RANK_5");
			break;
		case CHESS_RANK_6:
			printf("CHESS_RANK_6");
			break;
		case CHESS_RANK_7:
			printf("CHESS_RANK_7");
			break;
		case CHESS_RANK_8:
			printf("CHESS_RANK_8");
			break;
		default:
			printf("(ChessRank)%d", rank);
			break;
	}
}
bool chess_rank_is_valid(ChessRank rank) {
	return (rank & 0x8U) == 0;
}
size_t chess_rank_from_algebraic(ChessRank *rank, const char *string) {
	assert(rank != CHESS_NULL);
	assert(string != CHESS_NULL);

	if (string[0] < '1' || '8' < string[0]) {
		return 0;
	}

	*rank = (ChessRank)(CHESS_RANK_1 + (string[0] - '1'));

	assert(chess_rank_is_valid(*rank));

	return 1;
}
size_t chess_rank_to_algebraic(ChessRank rank, char *string, size_t string_size) {
	assert(chess_rank_is_valid(rank));
	assert(string != CHESS_NULL || string_size == 0);

	if (string != CHESS_NULL && string_size >= 2) {
		string[0] = (char)('1' + (rank - CHESS_RANK_1));
		string[1] = '\0';
	}

	return 1;
}
