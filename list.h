#include <stdlib.h>

struct list {
    struct list *next, *prev;
};

struct list* list_init(struct list *list);

struct list* list_insert(struct list *list, struct list *next);

struct list* list_remove(struct list *target);

struct list* list_first(struct list *last);

struct list* list_last(struct list *first);

void list_foreach(struct list *first, void (*func) (void*), size_t member_offset);

void* list_find(struct list *first, int (*func) (void*), size_t member_offset);

void* list_find_index(struct list *first, int (*func) (int, void*), size_t member_offset);

int list_len(struct list *first);

struct list* list_get(struct list *first, int index);

struct list* list_remove_at(struct list *first, int index);
