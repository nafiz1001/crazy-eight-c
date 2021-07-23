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
    
    list_init(&tmp, NULL);

    token = strtok(player_ids_data, WHITESPACE); 
    while (token != NULL) {
        players->id = token;
        players->cards = NULL;
        list_init(&players->players, players);
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
    struct list *stock_pile;
    struct list *discard_pile;

    player_list = NULL;
    stock_pile = NULL;
    discard_pile = NULL;

    // get player ids
    printf("Enter player ids: ");
    fgets(player_ids_data, sizeof(player_ids_data), stdin);

    player_list = initialize_players(players, player_ids_data); 
    
    // initialize cards
    for (int s = 0; s < 4; ++s) {
        for (int r = 1; r <= 13; ++r) {
            cards[s][r - 1].suit = s;
            cards[s][r - 1].rank = r;
        }
    } 
    
    stock_pile = list_init_from_array(&cards[0][0], &cards[0][0].cards, 4*13, sizeof(struct card));
    stock_pile = list_shuffle(stock_pile, time(NULL));

    srand((unsigned) time(NULL));

    for (int player_index = 0; player_index < list_len(player_list); ++player_index) {
        struct player *p = list_get(player_list, player_index)->owner;
        p->cards = stock_pile;
        stock_pile = list_insert_after(list_get(stock_pile, 4), NULL);
    }

    for (int player_index = 0; player_index < list_len(player_list); ++player_index) {
        struct player *p = list_get(player_list, player_index)->owner;

        printf("--- Player %s (#%d) ---\n", p->id, player_index);
        list_foreach(p->cards, print_card);
    }

    return 0;
}