#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define WHITESPACE " \t\n\v\f\r"

struct strings {
    char *data;
    int len;
};

void
strings_init(struct strings *s, char *data) {
    s->data = data;
    s->len = 0;
    
    char *token = strtok(s->data, WHITESPACE);
    while (token != NULL) {
        ++s->len;
        token = strtok(NULL, WHITESPACE);
    }
}

char*
strings_at(struct strings *s, int index) {
    assert(index < s->len);

    char *ptr = s->data;

    for (; isspace(*ptr); ++ptr);

    for (int i = 0; i < index; ++i) {
        for (; *ptr != '\0'; ++ptr);
        ++ptr;
    }

    return ptr;
}

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
    int player_index;
};

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

struct player {
    int index;
};

int
main(int argc, char *argv[]) {
    char player_ids_data[256];

    struct strings player_ids;
    struct player players[5];
    struct card cards[4][13];

    // get player ids
    printf("Enter player ids: ");
    fgets(player_ids_data, sizeof(player_ids_data), stdin);
    strings_init(&player_ids, player_ids_data);

    // initialize players
    for (int i = 0; i < player_ids.len; ++i) {
        players[i].index = i;
    }

    // initialize cards
    for (int s = 0; s < sizeof(cards)/sizeof(cards[0]); ++s) {
        for (int r = 0; r < sizeof(cards[s])/sizeof(cards[s][0]); ++r) {
            cards[s][r].suit = s;
            cards[s][r].rank = r;
            cards[s][r].player_index = -1;
        }
    }

    srand((unsigned) time(NULL));

    for (int player_index = 0; player_index < player_ids.len; ++player_index) {
        for (int c = 0; c < 5; ++c) {
            for(;;) {
                int card_index = rand() % sizeof(cards)/sizeof(cards[0][0]);
                struct card *card = &cards[card_index / 13][card_index % 13];
                if (card->player_index == -1) {
                    card->player_index = player_index;
                    break;
                }
            }
        }
    }

    for (int player_index = 0; player_index < player_ids.len; ++player_index) {
        printf("--- Player %s (#%d) ---\n", strings_at(&player_ids, player_index), player_index);
        for (int s = 0; s < sizeof(cards)/sizeof(cards[0]); ++s) {
            for (int r = 0; r < sizeof(cards[s])/sizeof(cards[s][0]); ++r) {
                if (cards[s][r].player_index == player_index) {
                    printf("  Suit: %8s Rank: %5s\n", suit_str[s], rank_str[r]);
                }
            }
        }
    }

    return 0;
}