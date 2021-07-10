#include "list.h"
#include <assert.h>
#include <stddef.h>

#define LIST_INIT_3(pfirst, pmid, plast, ret1, ret2) \
    struct list *ret1;\
    struct list *ret2;\
    list_init(pfirst);\
    list_init(pmid);\
    list_init(plast);\
    ret1 = list_insert(pfirst, pmid);\
    ret2 = list_insert(pmid, plast);


#define LIST_CREATE_3(first, mid, last, ret1, ret2) \
    struct list first;\
    struct list mid;\
    struct list last;\
    LIST_INIT_3(&first, &mid, &last, ret1, ret2)

void
test_init_list() {
    struct list list;
    list_init(&list);
    
    assert(list.next == NULL);
    assert(list.prev == NULL);
}

void 
test_list_insert_after() {
    LIST_CREATE_3(first, mid, last, ret1, ret2)

    assert(first.next == &mid);
    assert(first.prev == NULL);

    assert(mid.next == &last);
    assert(mid.prev == &first);
    
    assert(last.next == NULL);
    assert(last.prev == &mid);

    assert(ret1 == &mid);
    assert(ret2 == &last);
}

void 
test_list_insert_between() {
    struct list first;
    struct list mid;
    struct list last;
    struct list *ret1;
    struct list *ret2;
    list_init(&first);
    list_init(&mid);
    list_init(&last);
    ret1 = list_insert(&first, &last);
    ret2 = list_insert(&first, &mid);

    assert(first.next == &mid);
    assert(first.prev == NULL);

    assert(mid.next == &last);
    assert(mid.prev == &first);
    
    assert(last.next == NULL);
    assert(last.prev == &mid);

    assert(ret1 == &last);
    assert(ret2 == &mid);
}

void
test_remove() {
    LIST_CREATE_3(first, mid, last, ret1, ret2)

    list_remove(&mid);
    assert(first.next == &last);
    assert(first.prev == NULL);

    assert(mid.next == NULL);
    assert(mid.prev == NULL);

    assert(last.prev == &first);
    assert(last.next == NULL);

    list_remove(&last);
    assert(first.next == NULL);
    assert(first.prev == NULL);

    assert(last.prev == NULL);
    assert(last.next == NULL);

    assert(list_remove(&first) == &first);
}

void
test_list_len() {
    LIST_CREATE_3(first, mid, last, ret1, ret2)
    
    assert(list_len(&first) == 3);
    assert(list_len(&mid) == 2);
    assert(list_len(&last) == 1);
}

void
test_list_get() {
    LIST_CREATE_3(first, mid, last, ret1, ret2)
    
    assert(list_get(&first, 0) == &first);
    assert(list_get(&first, 1) == &mid);
    assert(list_get(&first, 2) == &last);
    assert(list_get(&first, 3) == NULL);
}

void
test_remove_at() {
    LIST_CREATE_3(first, mid, last, ret1, ret2)

    list_remove_at(&first, 1);
    assert(first.next == &last);
    assert(first.prev == NULL);

    assert(mid.next == NULL);
    assert(mid.prev == NULL);

    assert(last.prev == &first);
    assert(last.next == NULL);

    list_remove_at(&first, 1);
    assert(first.next == NULL);
    assert(first.prev == NULL);

    assert(last.prev == NULL);
    assert(last.next == NULL);

    assert(list_remove_at(&first, 0) == &first);
}

void
test_list_first() {
    LIST_CREATE_3(first, mid, last, ret1, ret2)
    
    assert(list_first(&last) == &first);
    assert(list_first(&first) == &first);
}

void
test_list_last() {
    LIST_CREATE_3(first, mid, last, ret1, ret2)
    
    assert(list_last(&first) == &last);
    assert(list_last(&last) == &last);
}

struct test_struct {
    int data;
    struct list list;
};

int
test_list_find_is_minus_two_index(int index, void *ptr) {
    return ((struct test_struct *)ptr)->data == -2;
}

int
test_list_find_is_minus_two(void *ptr) {
    return test_list_find_is_minus_two_index(0, ptr);
}

void
test_list_find() {
    struct test_struct structs[3];

    LIST_INIT_3(&structs[0].list, &structs[1].list, &structs[2].list, ret1, ret2)

    structs[0].data = -0;
    structs[1].data = -1;
    structs[2].data = -2;

    const size_t member_offset = offsetof(struct test_struct, list);
    assert(list_find(&structs[0].list, test_list_find_is_minus_two, member_offset) == &structs[2]);
    assert(list_find_index(&structs[0].list, test_list_find_is_minus_two_index, member_offset) == &structs[2]);
}

void
test_list_foreach_func(void *ptr) {
    struct test_struct *p = ptr;
    p->data = 100;
}

void
test_list_foreach() {
    struct test_struct structs[3];

    LIST_INIT_3(&structs[0].list, &structs[1].list, &structs[2].list, ret1, ret2)

    structs[0].data = 0;
    structs[1].data = 0;
    structs[2].data = 0;

    const size_t member_offset = offsetof(struct test_struct, list);
    list_foreach(&structs[0].list, test_list_foreach_func, member_offset);
    
    assert(structs[0].data == 100);
    assert(structs[1].data == 100);
    assert(structs[2].data == 100);
}

int
main() {
    test_init_list();
    test_list_insert_after();
    test_list_insert_between();
    test_remove();
    test_list_len();
    test_list_get();
    test_remove_at();
    test_list_first();
    test_list_last();
    test_list_find();
    test_list_foreach();
}