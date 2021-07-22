#include "list.h"
#include <assert.h>
#include <stdlib.h>

struct list* list_init(struct list *list, void *owner) {
    assert(list != NULL);

    list->next = NULL;
    list->prev = NULL;
    list->owner = owner;
    
    return list;
}

void list_insert_between(struct list *prev, struct list *mid, struct list *next) {
    if (prev != NULL) { 
        if (prev->next != NULL) {
            prev->next->prev = NULL;
        }
        prev->next = mid;
    }

    if (mid != NULL) {
        if (mid->prev != NULL) {
            mid->prev->next = NULL;
        }
        mid->prev  = prev;

        if (mid->next != NULL) {
            mid->next->prev = NULL;
        }
        mid->next  = next;
    }

    if (next != NULL) {
        if (next->prev != NULL) {
            next->prev->next = NULL;
        }
        next->prev = mid;
    }
}

struct list* list_insert_after(struct list *before, struct list *after) {
    assert(before != NULL);

    if (after == NULL) {
        struct list *tmp = before->next;
        list_insert_between(before, NULL, before->next);
        return tmp;
    } else {
        list_insert_between(before, after, before->next);
        return after;
    }
}

struct list* list_insert_before(struct list *before, struct list *after) {
    assert(after != NULL);

    if (before == NULL) {
        struct list *tmp = after->prev;
        list_insert_between(after->prev, NULL, after);
        return tmp;
    } else {
        list_insert_between(after->prev, before, after);
        return before;
    }
}

struct list* list_remove(struct list *target) {
    assert(target != NULL);

    if (target->prev != NULL) target->prev->next = target->next;
    if (target->next != NULL) target->next->prev = target->prev;
    
    target->prev = NULL;
    target->next = NULL;
    
    return target;
}


struct list* list_first(struct list *last) {
    assert(last != NULL);

    while (last->prev != NULL) {
        last = last->prev;
    }
    
    return last;
}

struct list* list_last(struct list *first) {
    assert(first != NULL);

    while (first->next != NULL) {
        first = first->next;
    }
    
    return first;
}

void list_foreach(struct list *first, void (*func) (void*)) {
    struct list **plist;

    assert(first != NULL);
    assert(func != NULL);

    plist = &first;

    while (*plist != NULL) {
        func((*plist)->owner);
        plist = &(*plist)->next;
    }
}

void* list_find(struct list *first, int (*func) (void*)) {
    struct list **plist;

    assert(first != NULL);
    assert(func != NULL);

    plist = &first;

    while (*plist != NULL) {
        if (func((*plist)->owner)) {
            return (*plist)->owner;
        }

        plist = &(*plist)->next;
    }
    
    return NULL;
}

void* list_find_index(struct list *first, int (*func) (int, void*)) {
    int index;
    struct list **plist;

    assert(first != NULL);
    assert(func != NULL);

    index = 0;
    plist = &first;

    while (*plist != NULL) {
        if (func(index, (*plist)->owner)) {
            return (*plist)->owner;
        }

        plist = &(*plist)->next;
        ++index;
    }
    
    return NULL;
}

int list_len(struct list *first) {
    int len;
    struct list **plist;

    assert(first != NULL);

    len = 0;
    plist = &first;

    while (*plist != NULL) {
        plist = &(*plist)->next;
        ++len;
    }

    return len;
}

struct list* list_get(struct list *first, int index) {
    int i;
    struct list **plist;

    assert(first != NULL);
    assert(index >= 0);

    i = 0;
    plist = &first;

    while (*plist != NULL) {
        if (i == index) {
            return *plist;
        }
        
        plist = &(*plist)->next;
        ++i;
    }
    
    assert(index < list_len(first));
    
    return NULL;
}

struct list* list_init_from_array(void *first, struct list *first_list, int len, size_t size) {
    void *iter = first;

    struct list *prev = first_list;
    list_init(prev, first);

    for (int i = 1; i < len; ++i) {
        iter = (char *)iter + size;
        struct list *next = (struct list *)((char *)prev + size);

        list_init(next, iter);
        list_insert_after(prev, next);
        prev = next;
    }
    
    return first_list;
}

struct list* list_shuffle(struct list *first, unsigned int seed) {
    srand(seed);
    int len = list_len(first);

    struct list *shuffled = NULL;

    while (len > 0) {
        struct list *next = list_get(first, rand() % len);
        if (next == first) {
            first = next->next;
        }

        if (!shuffled) {
            shuffled = next;
        } else {
            list_insert_after(shuffled, list_remove(next));
        }
        
        --len;
    }
}