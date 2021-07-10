#include "list.h"

struct list*
list_init(struct list *list) {
    list->next = NULL;
    list->prev = NULL;
    
    return list;
}

void __list_insert_between(struct list *prev, struct list *mid, struct list *next) {
    if (prev != NULL) prev->next = mid;
    mid->prev = prev;
    mid->next = next;
    if (next != NULL) next->prev = mid;
}

struct list*
list_insert(struct list *list, struct list *next) {
    __list_insert_between(list, next, list->next);
    return next;
}

struct list*
list_remove(struct list *target) {
    if (target->prev != NULL) target->prev->next = target->next;
    if (target->next != NULL) target->next->prev = target->prev;
    
    target->prev = NULL;
    target->next = NULL;
    
    return target;
}


struct list*
list_first(struct list *last) {
    while (last->prev != NULL) {
        last = last->prev;
    }
    
    return last;
}

struct list*
list_last(struct list *first) {
    while (first->next != NULL) {
        first = first->next;
    }
    
    return first;
}

void*
list_find(struct list *first, int (*func) (int, void*), size_t member_offset) {
    int index;
    struct list **plist;

    index = 0;
    plist = &first;

    while (*plist != NULL) {
        void *p = (char *)(*plist) - member_offset;
        if (func(index, p)) {
            return p;
        }

        plist = &(*plist)->next;
        ++index;
    }
    
    return NULL;
}

int
list_len(struct list *first) {
    int len;
    struct list **plist;

    len = 0;
    plist = &first;

    while (*plist != NULL) {
        plist = &(*plist)->next;
        ++len;
    }

    return len;
}

struct list*
list_get(struct list *first, int index) {
    int i;
    struct list **plist;

    i = 0;
    plist = &first;

    while (*plist != NULL) {
        if (i == index) {
            break;
        }
        
        plist = &(*plist)->next;
        ++i;
    }
    
    return *plist;
}

struct list*
list_remove_at(struct list *first, int index) {
    return list_remove(list_get(first, index));
}
