#include "card.h"
#include <stdio.h>

const char *suit_str[] = {
    "CLUB",
    "DIAMONDS",
    "HEARTS",
    "SPADE"
};

const char *rank_str[] = {
    "0",
    "ACE",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "10",
    "JACK",
    "QUEEN",
    "KING"
};

void print_card(void *card) {
    struct card *c = (struct card *) card;
    printf("  Suit: %-8s Rank: %-5s\n", suit_str[c->suit], rank_str[c->rank]);
}


int print_card_index(int index, void *card) {
    struct card *c = (struct card *) card;
    printf("%d Suit: %-8s Rank: %-5s\n", index, suit_str[c->suit], rank_str[c->rank]);
    return 0;
}