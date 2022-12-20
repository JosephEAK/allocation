#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../allocation.h"

#define ISNULL 0
#define ISNOTNULL 1

void tracker_values(buffer_tracker *head_tracker, int size, char filled, void *ptr, char NULL_next, char NULL_prev)
{
    CU_ASSERT(head_tracker->size == size);
    CU_ASSERT(head_tracker->filled == filled);

    if (ISNULL == NULL_next)
    {
        CU_ASSERT(head_tracker->next == NULL);
    }
    else
    {
        CU_ASSERT(head_tracker->next != NULL);
    }

    if (ISNULL == NULL_prev)
    {
        CU_ASSERT(head_tracker->prev == NULL);
    }
    else
    {
        CU_ASSERT(head_tracker->prev != NULL);
    }
}

void test_noob(void)
{
    buffer_tracker *head_tracker;
    init_heap();
    head_tracker = get_buffer_tracker();

    tracker_values(head_tracker, SIZE_HEAP, FREE_BLOCK, get_heap(), ISNULL, ISNULL);

    free_heap();
}

void test_heap_malloc_init()
{
    buffer_tracker *head_tracker;
    void *heap;

    init_heap();

    head_tracker = get_buffer_tracker();
    heap = get_heap();

    unsigned int ptr_size = 12;
    char *ptr = (char *)allocate_memory(ptr_size * sizeof(char));
    double_linked_list *track;

    CU_ASSERT(ptr == heap);

    track = head_tracker;

    CU_ASSERT(track->ptr == ptr);
    CU_ASSERT(track->size == ptr_size);

    track = track->next;
    CU_ASSERT(track->ptr == ptr + ptr_size);
    CU_ASSERT(track->size == SIZE_HEAP - ptr_size);
    CU_ASSERT(track->filled == FREE_BLOCK);

    free_heap();
}

void test_add_several(void)
{
    buffer_tracker *head_tracker;
    init_heap();

    head_tracker = get_buffer_tracker();

    unsigned int ptr_size1 = 12;
    unsigned int ptr_size2 = 85;

    char *ptr1 = (char *)allocate_memory(ptr_size1 * sizeof(char));
    char *ptr2 = (char *)allocate_memory(ptr_size2 * sizeof(char));

    strcpy(ptr1, "hello");
    strcpy(ptr2, "hello");

    tracker_values(head_tracker, ptr_size1, ALLOCATED, get_heap(), ISNOTNULL, ISNULL);
    head_tracker = head_tracker->next;
    tracker_values(head_tracker, ptr_size2, ALLOCATED, get_heap(), ISNOTNULL, ISNOTNULL);

    free_heap();
}

void test_free(void)
{
    buffer_tracker *head_tracker;
    init_heap();

    head_tracker = get_buffer_tracker();

    unsigned int ptr_size1 = 12;

    char *ptr1 = (char *)allocate_memory(ptr_size1 * sizeof(char));
    strcpy(ptr1, "hello");

    free_memory(ptr1);

    tracker_values(head_tracker, SIZE_HEAP, FREE_BLOCK, get_heap(), ISNULL, ISNULL);

    free_heap();
}

void test_free_last_one(void)
{
    buffer_tracker *head_tracker;
    void *heap;

    init_heap();

    head_tracker = get_buffer_tracker();
    heap = get_heap();

    unsigned int ptr_size1 = 9;
    unsigned int ptr_size2 = 11;
    unsigned int ptr_size3 = 44;
    unsigned int ptr_size4 = 16;

    char *ptr1 = (char *)allocate_memory(ptr_size1 * sizeof(char));
    char *ptr2 = (char *)allocate_memory(ptr_size2 * sizeof(char));
    char *ptr3 = (char *)allocate_memory(ptr_size3 * sizeof(char));
    char *ptr4 = (char *)allocate_memory(ptr_size4 * sizeof(char));

    strcpy(ptr1, "hello");
    strcpy(ptr2, "hello");
    strcpy(ptr3, "hello");
    strcpy(ptr4, "hello");

    free_memory(ptr4);

    tracker_values(head_tracker, ptr_size1, ALLOCATED, get_heap(), ISNOTNULL, ISNULL);

    head_tracker = head_tracker->next;

    tracker_values(head_tracker, ptr_size2, ALLOCATED, get_heap(), ISNOTNULL, ISNOTNULL);

    head_tracker = head_tracker->next;

    tracker_values(head_tracker, ptr_size3, ALLOCATED, get_heap(), ISNOTNULL, ISNOTNULL);

    head_tracker = head_tracker->next;

    tracker_values(head_tracker,
                   SIZE_HEAP - ptr_size1 - ptr_size2 - ptr_size3,
                   FREE_BLOCK,
                   (char *)heap + ptr_size1 + ptr_size2 + ptr_size3,
                   ISNULL,
                   ISNOTNULL);

    free_heap();
}

void test_free_middle(void)
{
    buffer_tracker *head_tracker;
    // void *heap;

    init_heap();

    head_tracker = get_buffer_tracker();
    // heap = get_heap();

    unsigned int ptr_size1 = 9;
    unsigned int ptr_size2 = 11;
    unsigned int ptr_size3 = 44;
    unsigned int ptr_size4 = 16;

    char *ptr1 = (char *)allocate_memory(ptr_size1 * sizeof(char));
    char *ptr2 = (char *)allocate_memory(ptr_size2 * sizeof(char));
    char *ptr3 = (char *)allocate_memory(ptr_size3 * sizeof(char));
    char *ptr4 = (char *)allocate_memory(ptr_size4 * sizeof(char));

    strcpy(ptr1, "hello");
    strcpy(ptr2, "hello");
    strcpy(ptr3, "hello");
    strcpy(ptr4, "hello");

    free_memory(ptr2);
    print_head_tracker();

    tracker_values(head_tracker, 9, ALLOCATED, get_heap(), ISNOTNULL, ISNULL);

    head_tracker = head_tracker->next;

    tracker_values(head_tracker, 9, FREE_BLOCK, get_heap(), ISNOTNULL, ISNOTNULL);

    free_memory(ptr3);
    head_tracker = get_buffer_tracker();
    head_tracker = head_tracker->next;

    tracker_values(head_tracker,
                   ptr_size3 + ptr_size2,
                   FREE_BLOCK,
                   get_heap(),
                   ISNOTNULL,
                   ISNOTNULL);

    print_head_tracker();

    free_heap();
}

void test_free_left(void)
{
    buffer_tracker *head_tracker;

    init_heap();

    head_tracker = get_buffer_tracker();

    unsigned int ptr_size1 = 9;
    unsigned int ptr_size2 = 11;
    unsigned int ptr_size3 = 44;
    unsigned int ptr_size4 = 16;

    char *ptr1 = (char *)allocate_memory(ptr_size1 * sizeof(char));
    char *ptr2 = (char *)allocate_memory(ptr_size2 * sizeof(char));
    char *ptr3 = (char *)allocate_memory(ptr_size3 * sizeof(char));
    char *ptr4 = (char *)allocate_memory(ptr_size4 * sizeof(char));

    strcpy(ptr1, "hello");
    strcpy(ptr2, "hello");
    strcpy(ptr3, "hello");
    strcpy(ptr4, "hello");

    free_memory(ptr2);
    print_head_tracker();

    tracker_values(head_tracker, 9, ALLOCATED, get_heap(), ISNOTNULL, ISNULL);

    print_head_tracker();

    free_heap();
}

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

int main()
{
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_1", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if (NULL == CU_add_test(pSuite, "test_noob()", test_noob) ||
        NULL == CU_add_test(pSuite, "test_heap_malloc_init()", test_heap_malloc_init) ||
        NULL == CU_add_test(pSuite, "test_add_several()", test_add_several) ||
        NULL == CU_add_test(pSuite, "test_free()", test_free) ||
        NULL == CU_add_test(pSuite, "test_free_last_one()", test_free_last_one) ||
        NULL == CU_add_test(pSuite, "test_free_middle()", test_free_middle) ||
        NULL == CU_add_test(pSuite, "test_free_left()", test_free_left))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
