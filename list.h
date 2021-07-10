#include <stdlib.h>

struct list {
    struct list *next, *prev;
};

struct list*
list_init(struct list *list);

struct list*
list_insert(struct list *list, struct list *next);

struct list*
list_remove(struct list *target);

struct list*
list_first(struct list *list);

struct list*
list_last(struct list *list);

struct list*
list_foreach(struct list *list, int (*func) (int, void*), size_t member_offset);

int
list_len(struct list *list);

struct list*
list_get(struct list *list, int index);

struct list*
list_remove_at(struct list *list, int index);
