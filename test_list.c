#include "list.h"
#include <assert.h>
#include <stddef.h>

#define LIST_INIT_3(pfirst, pmid, plast, ret1, ret2) \
    struct list *ret1;\
    struct list *ret2;\
    list_init(pfirst, NULL);\
    list_init(pmid, NULL);\
    list_init(plast, NULL);\
    ret1 = list_insert_after(pfirst, plast);\
    ret2 = list_insert_after(pfirst, pmid);

#define LIST_CREATE_3(first, mid, last, ret1, ret2) \
    struct list first;\
    struct list mid;\
    struct list last;\
    LIST_INIT_3(&first, &mid, &last, ret1, ret2);

#define ASSERT_PREV_MID_RIGHT_AFTER_INSERTIONS(first, mid, last) \
    assert(first.next == &mid);\
    assert(first.prev == NULL);\
    assert(mid.next == &last);\
    assert(mid.prev == &first);\
    assert(last.next == NULL);\
    assert(last.prev == &mid);

void test_init_list() {
    struct list list;
    list_init(&list, NULL);
    
    assert(list.next == NULL);
    assert(list.prev == NULL);
}

void test_list_insert_after() {
    LIST_CREATE_3(first, mid, last, ret1, ret2);

    ASSERT_PREV_MID_RIGHT_AFTER_INSERTIONS(first, mid, last);

    assert(ret1 == &last);
    assert(ret2 == &mid);

    assert(list_insert_after(&first, NULL) == &mid);
    assert(first.next == NULL);
    assert(mid.prev == NULL);
}

void test_list_insert_before() {
    struct list first;
    struct list mid;
    struct list last;
    struct list *ret1;
    struct list *ret2;
    list_init(&first, NULL);
    list_init(&mid, NULL);
    list_init(&last, NULL);
    ret1 = list_insert_before(&first, &last);
    ret2 = list_insert_before(&mid, &last);
    
    ASSERT_PREV_MID_RIGHT_AFTER_INSERTIONS(first, mid, last);

    assert(ret1 == &first);
    assert(ret2 == &mid);

    assert(list_insert_before(NULL, &last) == &mid);
    assert(mid.next == NULL);
    assert(last.prev == NULL);
}

void test_list_insert_between() {
    struct list first;
    struct list mid;
    struct list last;
    list_init(&first, NULL);
    list_init(&mid, NULL);
    list_init(&last, NULL);
    list_insert_between(&first, &mid, &last);

    ASSERT_PREV_MID_RIGHT_AFTER_INSERTIONS(first, mid, last);
    
    list_insert_between(&first, NULL, &mid);
    assert(first.next == NULL);
    assert(mid.prev   == NULL);

    list_insert_between(NULL, NULL, &last);
    assert(last.prev == NULL);
    assert(mid.next  == NULL);
}

void test_remove() {
    LIST_CREATE_3(first, mid, last, ret1, ret2);

    list_remove(&mid);
    assert(first.next == &last);

    assert(mid.next == NULL);
    assert(mid.prev == NULL);

    assert(last.prev == &first);

    list_remove(&last);
    assert(first.next == NULL);

    assert(last.prev == NULL);

    assert(list_remove(&first) == &first);
}

void test_list_len() {
    LIST_CREATE_3(first, mid, last, ret1, ret2);
    
    assert(list_len(&first) == 3);
    assert(list_len(&mid) == 2);
    assert(list_len(&last) == 1);
}

void test_list_get() {
    LIST_CREATE_3(first, mid, last, ret1, ret2);
    
    assert(list_get(&first, 0) == &first);
    assert(list_get(&first, 1) == &mid);
    assert(list_get(&first, 2) == &last);
}

void test_list_first() {
    LIST_CREATE_3(first, mid, last, ret1, ret2);
    
    assert(list_first(&last) == &first);
    assert(list_first(&first) == &first);
}

void test_list_last() {
    LIST_CREATE_3(first, mid, last, ret1, ret2);
    
    assert(list_last(&first) == &last);
    assert(list_last(&last) == &last);
}

struct test_struct {
    int data;
    struct list list;
};

int is_minus_two_index(int index, void *ptr) {
    return ((struct test_struct *)ptr)->data == -2 && index == 2;
}

int is_minus_two(void *ptr) {
    return ((struct test_struct *)ptr)->data == -2;
}

void test_list_find() {
    struct test_struct structs[3];

    LIST_INIT_3(&structs[0].list, &structs[1].list, &structs[2].list, ret1, ret2);
    
    structs[0].list.owner = &structs[0];
    structs[1].list.owner = &structs[1];
    structs[2].list.owner = &structs[2];

    structs[0].data = -0;
    structs[1].data = -1;
    structs[2].data = -2;

    const size_t member_offset = offsetof(struct test_struct, list);
    assert(list_find(&structs[0].list, is_minus_two) == &structs[2]);
    assert(list_find_index(&structs[0].list, is_minus_two_index) == &structs[2]);
}

void
test_list_foreach_func(void *ptr) {
    struct test_struct *p = ptr;
    p->data = 100;
}

void test_list_foreach() {
    struct test_struct structs[3];

    LIST_INIT_3(&structs[0].list, &structs[1].list, &structs[2].list, ret1, ret2);

    structs[0].list.owner = &structs[0];
    structs[1].list.owner = &structs[1];
    structs[2].list.owner = &structs[2];

    structs[0].data = 0;
    structs[1].data = 0;
    structs[2].data = 0;

    const size_t member_offset = offsetof(struct test_struct, list);
    list_foreach(&structs[0].list, test_list_foreach_func);
    
    assert(structs[0].data == 100);
    assert(structs[1].data == 100);
    assert(structs[2].data == 100);
}

void test_list_init_from_array() {
    struct test_struct structs[4];
    assert(
        list_init_from_array(&structs[0], &structs[0].list, 4, sizeof(struct test_struct))
        ==
        &structs[0].list
    );
    
    assert(structs[0].list.prev == NULL);
    assert(structs[0].list.next == &structs[1].list);
    
    for (int i = 1; i < 3; ++i) {
        assert(structs[i].list.prev == &structs[i - 1].list);
        assert(structs[i].list.next == &structs[i + 1].list);
    }
    
    assert(structs[3].list.prev == &structs[2].list);
    assert(structs[3].list.next == NULL);
}

int main() {
    test_init_list();
    test_list_insert_after();
    test_list_insert_before();
    test_list_insert_between();
    test_remove();
    test_list_len();
    test_list_get();
    test_list_first();
    test_list_last();
    test_list_find();
    test_list_foreach();
    test_list_init_from_array();
}