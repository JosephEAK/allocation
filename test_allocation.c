#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "allocation.h"

void test_tas_malloc_example()
{
    init_tas();
    char *p1 = tas_malloc(10);
    strcpy(p1, "tp 1");

    CU_ASSERT(p1 - 1 == tas);
    CU_ASSERT(*(p1 + 1) != FREE_BLOCK);

    CU_ASSERT(libre == 11);
    CU_ASSERT(*(tas + libre) == 116);
    CU_ASSERT(*(tas + libre + 1) == FREE_BLOCK);

    char *p2 = tas_malloc(9);
    strcpy(p2, "tp 2");

    CU_ASSERT(p2 == tas + 12);
    CU_ASSERT(*(p2 - 1) == 9);
    CU_ASSERT(libre == 21);

    char *p3 = tas_malloc(5);
    strcpy(p3, "tp 3");

    CU_ASSERT(p3 == tas + 22);
    CU_ASSERT(*(p3 - 1) == 5);
    CU_ASSERT(libre == 27);

    char *p4 = tas_malloc(101);
    CU_ASSERT(p4 == NULL);
}

void test_tas_free_several()
{
    init_tas();
    char *p1 = tas_malloc(10);
    char *p2 = tas_malloc(10);
    char *p3 = tas_malloc(10);
    char *p4 = tas_malloc(10);

    strcpy(p1, "tp1");
    strcpy(p2, "tp2");
    strcpy(p3, "tp3");
    strcpy(p4, "tp4");

    tas_free(p2); // simple free

    CU_ASSERT(*(p2 - 1) == 10);
    CU_ASSERT(*(p2) == FREE_BLOCK);
    CU_ASSERT(libre == p2 - 1 - tas);

    tas_free(p3); // testing merge left

    CU_ASSERT(*(p3) == INIT_VAL);
    CU_ASSERT(*(p3 - 1) == INIT_VAL);

    CU_ASSERT(*(p2 - 1) == 21);
    CU_ASSERT(*(p2) == FREE_BLOCK);
    CU_ASSERT(libre == p2 - 1 - tas);

    tas_free(p1); // testing merge right

    CU_ASSERT(*(p1 - 1) == 32);
    CU_ASSERT(*(p1) == FREE_BLOCK);
    CU_ASSERT(libre == 0);
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
    if (
        NULL == CU_add_test(pSuite, "test of test_tas_free_several()", test_tas_free_several) ||
        NULL == CU_add_test(pSuite, "test of test_tas_malloc_example()", test_tas_malloc_example))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
