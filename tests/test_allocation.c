#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../allocation.h"

void test_noob(void)
{
    buffer_tracker *head_tracker;
    init_heap();
    head_tracker = get_buffer_tracker();

    CU_ASSERT(head_tracker->size == SIZE_HEAP);
    CU_ASSERT(head_tracker->filled == FREE_BLOCK);
    CU_ASSERT(head_tracker->next == NULL);
    CU_ASSERT(head_tracker->prev == NULL);

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

    CU_ASSERT(head_tracker->size == ptr_size1);
    CU_ASSERT(head_tracker->ptr == ptr1);

    CU_ASSERT(head_tracker->next->size == ptr_size2);
    CU_ASSERT(head_tracker->next->ptr == ptr2);

    free_heap();
}

void test_free_several(void)
{
    buffer_tracker *head_tracker;
    init_heap();
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
        NULL == CU_add_test(pSuite, "test_free_several()", test_free_several))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
