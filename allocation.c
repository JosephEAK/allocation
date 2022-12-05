#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "allocation.h"

void init_tas(void)
{
    int i;

    for (i = 0; i < TAILLE_TAS; i++)
    { // Might be test only at some point
        tas[i] = INIT_VAL;
    }

    tas[0] = TAILLE_TAS - 1;
    tas[1] = FREE_BLOCK;
    libre = 0;
    count = 0;
}

char *tas_malloc(unsigned int taille)
{
    char *pt;
    char prev_libre;

    if (tas[libre] - 1 >= taille)
    {
        printf("Memory allocated.\n");

        pt = tas + libre + 1;
        prev_libre = tas[libre];
        *(pt - 1) = taille;
        libre = taille + libre + 1;
        tas[libre] = prev_libre - taille + FREE_BLOCK;
        tas[libre + 1] = FREE_BLOCK;
        return pt;
    }
    else
    {
        printf("Memory Not allocated.\n");
        return NULL;
    }
}

int get_index_on_tas(char *ptr)
{
    return ptr - tas;
}

char get_block_size(char *ptr)
{
    return *(ptr - 1);
}

char *first_fit(unsigned int size)
{

    int jump;
    char *ptr = tas + libre;
    if (*ptr >= size)
    {
        return ptr;
    }
    while (ptr < tas + TAILLE_TAS - 1)
    { // Walks through the tas
        // if the current block is free and has the proper size
        if (*ptr >= size && *(ptr + 1) == FREE_BLOCK)
        {
            return ptr;
        }
        jump = *ptr + 1;
        ptr = ptr + jump;
        /*printf("moving to ptr + %d\n", jump);*/
    }
    return NULL;
}

int find_new_libre(char *ptr)
{
    int jump;

    while (ptr < tas + TAILLE_TAS - 1)
    { // Walks through the tas
        if (*(ptr + 1) == FREE_BLOCK)
        { // if found a free block return its index
            return get_index_on_tas(ptr);
        }
        jump = *ptr + 1;
        ptr = ptr + jump;
    }
    return MEMORY_FULL;
}

void merge_free_blocks(char *block_left, char *block_right)
{
    char block_l_size = *block_left;
    char block_r_size = *block_right;

    *block_right = INIT_VAL;
    *(block_right + 1) = INIT_VAL;

    *block_left = block_l_size + block_r_size + 1;
}

void right_merge_free_blocks(char *ptr)
{
    char size_block = get_block_size(ptr);
    char *next_block;
    char val_next_block;

    next_block = ptr + size_block;
    val_next_block = *(next_block + 1);

    if (val_next_block == FREE_BLOCK)
    { // it means we have to merge the blocks
        merge_free_blocks(ptr - 1, next_block);
    }
}

void left_merge_free_blocks(char *ptr)
{
    int index_ptr;
    char *ptr_left;
    char size_left_free_block;

    ptr_left = ptr - 1;
    while (*ptr_left != FREE_BLOCK && ptr_left > tas)
    { // rewind to the first FREE_BLOCK
        ptr_left--;
    }

    if (ptr_left > tas)
    {
        size_left_free_block = *(ptr_left - 1);
        if (ptr_left + size_left_free_block + 1 == ptr)
        {
            // We shall merge the blocks, the first free block on the left is glued to this one
            merge_free_blocks(ptr_left - 1, ptr - 1);
            // Let's update libre with the new block
            index_ptr = get_index_on_tas(ptr_left - 1);
            if (index_ptr < libre)
            {
                libre = index_ptr;
            }
        }
    }
}

void tas_free(char *ptr)
{
    int index_ptr;

    *(ptr) = FREE_BLOCK;

    right_merge_free_blocks(ptr);

    index_ptr = get_index_on_tas(ptr - 1);
    if (index_ptr < libre)
    {
        libre = index_ptr;
        /*printf("updating libre, libre = %d\n", libre);*/
    }

    // Now we need to look left to see if a block might be merged
    left_merge_free_blocks(ptr);
}

void print_tas(void)
{
    printf("libre = %d\n", libre);
    int i, j;
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 16; j++)
        {
            printf("%4d", j + 16 * i);
        }
        printf("\n");
        for (j = 0; j < 16; j++)
        {
            printf("%4d", tas[j + 16 * i]);
        }
        printf("\n");
        for (j = 0; j < 16; j++)
        {
            if (isprint(tas[j + 16 * i]))
            {
                printf("%4c", tas[j + 16 * i]);
            }
            else
            {
                printf("    ");
            }
        }
        printf("\n\n");
    }
    printf("---------------------------------------------------------------\n\n");
}

char *best_fit_(unsigned int size)
{
    int jump;
    char *ptr = tas + libre;
    if (*ptr >= size)
    {
        return ptr;
    }
    while (ptr < tas + TAILLE_TAS - 1)
    { // Walks through the tas
        // if the current block is free and has the proper size
        if (*ptr >= size && *(ptr + 1) == FREE_BLOCK)
        {
            return ptr;
        }
        jump = *ptr + 1;
        ptr = ptr + jump;
        /*printf("moving to ptr + %d\n", jump);*/
    }
    return NULL;
}

char *first_fit_(unsigned int size)
{
    int jump;
    char *ptr = tas + libre;
    if (*ptr >= size)
    {
        return ptr;
    }
    while (ptr < tas + TAILLE_TAS - 1)
    { // Walks through the tas
        // if the current block is free and has the proper size
        if (*ptr >= size && *(ptr + 1) == FREE_BLOCK)
        {
            return ptr;
        }
        jump = *ptr + 1;
        ptr = ptr + jump;
        /*printf("moving to ptr + %d\n", jump);*/
    }
    return NULL;
}

void set_strategy(char *(*strat)(unsigned int size))
{
    strategy = strat;
}