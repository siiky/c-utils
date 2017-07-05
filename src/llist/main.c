#include <assert.h>

#include <stdio.h>
#include <stddef.h>

#define LLIST_DATA_TYPE size_t
#define LLIST_PREFIX llist_
#include <llist.h>

size_t print_shit (size_t ret)
{
    printf("%zu ", ret);
    return ret;
}

void test_append (struct llist_LList * list, size_t n)
{
    for (size_t i = 0; i < n; i++)
        llist_append(list, i);

    llist_map(list, print_shit);
    putchar('\n');

    size_t len = llist_len(list);

    for (size_t i = 0; i < len; i++)
        printf("%zu ", llist_nth(list, i));
    putchar('\n');

    for (size_t i = 0; i < len; i++)
        assert(llist_nth(list, i) == i);

    llist_free(list);

    assert(list->length == 0);
    assert(list->head == NULL);
    assert(list->last == NULL);
}

void test_cons (struct llist_LList * list, size_t n)
{
    for (size_t i = 0; i < n; i++)
        llist_append(list, i);

    llist_map(list, print_shit);
    putchar('\n');

    size_t len = llist_len(list);

    for (size_t i = 0; i < len; i++)
        printf("%zu ", llist_nth(list, i));
    putchar('\n');

    for (size_t i = 0; i < len; i++)
        assert(llist_nth(list, i) == (len - i - 1));

    llist_free(list);

    assert(list->length == 0);
    assert(list->head == NULL);
    assert(list->last == NULL);
}

int main (void)
{
    struct llist_LList list = llist_new();

    test_append(&list, 151);
    putchar('\n');
    putchar('\n');
    test_cons(&list, 151);

    return 0;
}
