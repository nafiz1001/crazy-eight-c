#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include "list.h"
#include "card.h"

#define WHITESPACE " \t\n\v\f\r"

struct player {
    char *id;
    struct list players;
    struct list *cards;
};

struct list* initialize_players(struct player *players, char *player_ids_data) {
    char *token; 
    struct list tmp;
    
    list_init(&tmp);

    token = strtok(player_ids_data, WHITESPACE); 
    while (token != NULL) {
        players->id = token;
        players->cards = NULL;
        list_init(&players->players);
        list_insert_after(list_last(&tmp), &players->players);

        ++players;
        token = strtok(NULL, WHITESPACE);
    }
    
    return list_insert_after(&tmp, NULL);
}

int
main(int argc, char *argv[]) {
    char player_ids_data[256];

    struct player players[5];
    struct card cards[4][13];
    
    struct list *player_list;
    struct card *stock_pile;
    struct card *discard_pile;

    player_list = NULL;
    stock_pile = NULL;
    discard_pile = NULL;

    // get player ids
    printf("Enter player ids: ");
    fgets(player_ids_data, sizeof(player_ids_data), stdin);

    player_list = initialize_players(players, player_ids_data); 
    const int player_len = list_len(player_list);

    // initialize cards
    for (int s = 0; s < sizeof(cards)/sizeof(cards[0]); ++s) {
        for (int r = 0; r < sizeof(cards[s])/sizeof(cards[s][0]); ++r) {
            cards[s][r].suit = s;
            cards[s][r].rank = r + 1;
            list_init(&cards[s][r].cards);
        }
    }

    srand((unsigned) time(NULL));

    for (int player_index = 0; player_index < player_len; ++player_index) {
        struct player *p = GET_LIST_OWNER(list_get(player_list, player_index), struct player, players);

        for (int c = 0; c < 5; ++c) {
            for(;;) {
                int card_index = rand() % sizeof(cards)/sizeof(cards[0][0]);
                struct card *card = &cards[card_index / 13][card_index % 13];
                
                if (card->cards.prev == NULL && card->cards.next == NULL) {
                    if (p->cards == NULL) {
                        p->cards = &card->cards;
                    } else {
                        p->cards = list_insert_before(&card->cards, p->cards);
                    }
                    break;
                }
            }
        }
    }

    for (int player_index = 0; player_index < player_len; ++player_index) {
        struct player *p = GET_LIST_OWNER(list_get(player_list, player_index), struct player, players);

        printf("--- Player %s (#%d) ---\n", p->id, player_index);
        list_foreach(p->cards, print_card, offsetof(struct card, cards));
    }

    return 0;
}