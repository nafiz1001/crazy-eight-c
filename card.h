#ifndef CARD_H
#define CARD_H

#include "list.h"

enum suit {
    CLUB,
    DIAMONDS,
    HEARTS,
    SPADE
};

enum rank {
    ACE = 1,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING
};

struct card {
    enum suit suit;
    enum rank rank;
    struct list cards;
};

void print_card(void *card);

int print_card_index(int index, void *card);

#endif