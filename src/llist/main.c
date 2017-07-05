#include <assert.h>

#include <stdio.h>
#include <stddef.h>

#define LLIST_DATA_TYPE size_t
#define LLIST_PREFIX llist_
#include <llist.h>

size_t print_shit (size_t ret)
{
    printf("%zu\t", ret);
    return ret;
}

int main (void)
{
    struct llist_LList list = llist_new();

    for (size_t i = 0; i < 10; i++)
        llist_append(&list, i);

    llist_map(&list, print_shit);
    putchar('\n');

    size_t len = llist_len(&list);
    for (size_t i = 0; i < len; i++) {
        printf("%u\t", llist_nth(&list, i));
        //assert(llist_nth(&list, i) == i);
    }
    putchar('\n');

    llist_free(&list);

    assert(list.length == 0);
    assert(list.head == NULL);
    assert(list.last == NULL);

    return 0;
}
