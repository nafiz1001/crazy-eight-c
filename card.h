#ifndef CARD_H
#define CARD_H

#include <stddef.h>
#include <sys/queue.h>

enum suit { CLUB, DIAMONDS, HEARTS, SPADE };

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
  TAILQ_ENTRY(card) entries;
};
TAILQ_HEAD(card_head, card);

const char *const SUIT_STRING[] = {"CLUB", "DIAMONDS", "HEARTS", "SPADE"};

const char *const RANK_STRING[] = {NULL, "ACE",  "2",     "3",   "4",
                                "5",  "6",    "7",     "8",   "9",
                                "10", "JACK", "QUEEN", "KING"};

#endif