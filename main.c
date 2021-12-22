#include "card.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <time.h>

#define WHITESPACE " \t\n\v\f\r"

struct player {
	const char *id;
	struct card_head card_head;
	TAILQ_ENTRY(player) entries;
};
TAILQ_HEAD(player_head, player);

int main()
{
	struct player players_buffer[5];
	struct card cards_buffer[4][13];

	struct player_head players;
	struct card_head stock_pile;
	struct card_head discard_pile;

	TAILQ_INIT(&players);
	TAILQ_INIT(&stock_pile);
	TAILQ_INIT(&discard_pile);

	// get player ids
	printf("Enter player ids:\n");
	char *player_id = NULL;
	size_t player_id_len = 0;
	size_t index = 0;
	printf("0. ");
	while (getline(&player_id, &player_id_len, stdin) > 1) {
		for (size_t i = 0; i < player_id_len; i++) {
			if (player_id[i] == '\n' || player_id[i] == '\r') {
				player_id[i] = '\0';
			}
		}

		TAILQ_INSERT_TAIL(&players, &players_buffer[index], entries);
		TAILQ_LAST(&players, player_head)->id = player_id;
		TAILQ_INIT(&TAILQ_LAST(&players, player_head)->card_head);

		player_id = NULL;
		index++;
		printf("%ld. ", index);
	}

	// initialize cards
	for (int s = 0; s < 4; ++s) {
		for (int r = 1; r <= 13; ++r) {
			cards_buffer[s][r - 1].suit = s;
			cards_buffer[s][r - 1].rank = r;
			TAILQ_INSERT_TAIL(&stock_pile, &cards_buffer[s][r - 1],
					  entries);
		}
	}

	srand((unsigned)time(NULL));

	for (int j = 0; j < 10; j++) {
		struct card *card_iter = TAILQ_FIRST(&stock_pile);
		for (int i = 0; i < 4 * 13; i++) {
			struct card *card_temp = TAILQ_NEXT(card_iter, entries);
			if (rand() % 2) {
				TAILQ_REMOVE(&stock_pile, card_iter, entries);
				TAILQ_INSERT_TAIL(&stock_pile, card_iter,
						  entries);
			}
			card_iter = card_temp;
		}
	}

	struct player *player_iter = NULL;
	TAILQ_FOREACH(player_iter, &players, entries)
	{
		for (int i = 0; i < 4; i++) {
			struct card *first_card = TAILQ_FIRST(&stock_pile);
			TAILQ_REMOVE(&stock_pile, first_card, entries);
			TAILQ_INSERT_TAIL(&player_iter->card_head, first_card,
					  entries);
		}
	}

	player_iter = NULL;
	index = 0;
	TAILQ_FOREACH(player_iter, &players, entries)
	{
		printf("--- Player %s (#%ld) ---\n", player_iter->id, index);
		const struct card *card_iter = NULL;
		size_t card_index = 0;
		TAILQ_FOREACH(card_iter, &player_iter->card_head, entries)
		{
			printf("%ld Suit: %-8s Rank: %-5s\n", card_index,
			       SUIT_STRING[card_iter->suit],
			       RANK_STRING[card_iter->rank]);
			card_index++;
		}
		index++;
	}
	printf("------------------\n");

	// player_iter = NULL;
	// TAILQ_FOREACH(player_iter, &players, entries) {
	// 	card_iter = NULL;
	// 	TAILQ_FOREACH(card_iter, &player_iter->card_head, entries) {
	// 		printf("Suit: %-8s Rank: %-5s\n", SUIT_STRING[card_iter->suit], RANK_STRING[card_iter->rank]);
	// 	}
	// }

	// card_iter = NULL;
	// TAILQ_FOREACH(card_iter, &stock_pile, entries) {
	// 	printf("Suit: %-8s Rank: %-5s\n", SUIT_STRING[card_iter->suit], RANK_STRING[card_iter->rank]);
	// }

	return 0;
}