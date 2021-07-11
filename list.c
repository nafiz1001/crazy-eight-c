#include "list.h"
#include <assert.h>

struct list* list_init(struct list *list) {
    assert(list != NULL);

    list->next = NULL;
    list->prev = NULL;
    
    return list;
}

void __list_insert_between(struct list *prev, struct list *mid, struct list *next) {
    if (prev != NULL) prev->next = mid;
    if (mid  != NULL) mid->prev = prev;
    if (mid  != NULL) mid->next = next;
    if (next != NULL) next->prev = mid;
}

struct list* list_insert(struct list *left, struct list *right) {
    if (left == NULL) {

        assert(left == NULL && right != NULL);
        struct list *tmp = right->prev;
        __list_insert_between(right->prev, NULL, right);
        return tmp;

    } else if (right == NULL) {

        struct list *tmp = left->next;
        __list_insert_between(left, NULL, left->next);
        return tmp;

    } else {

        __list_insert_between(left, right, left->next);
        return right;

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

void list_foreach(struct list *first, void (*func) (void*), size_t member_offset) {
    struct list **plist;

    assert(first != NULL);
    assert(func != NULL);

    plist = &first;

    while (*plist != NULL) {
        func((char *)(*plist) - member_offset);
        plist = &(*plist)->next;
    }
}

void* list_find(struct list *first, int (*func) (void*), size_t member_offset) {
    struct list **plist;

    assert(first != NULL);
    assert(func != NULL);

    plist = &first;

    while (*plist != NULL) {
        void *p = (char *)(*plist) - member_offset;
        if (func(p)) {
            return p;
        }

        plist = &(*plist)->next;
    }
    
    return NULL;
}

void* list_find_index(struct list *first, int (*func) (int, void*), size_t member_offset) {
    int index;
    struct list **plist;

    assert(first != NULL);
    assert(func != NULL);

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

struct list* list_remove_at(struct list *first, int index) {
    assert(first != NULL);
    assert(index >= 0);
    return list_remove(list_get(first, index));
}
