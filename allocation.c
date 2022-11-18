#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "allocation.h"

void init_tas(void)
{
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

char *tas_free(char *pt)
{
    int i;
    char *pt1, *pt2;
    int temp1, temp2, slot = 0;
    *pt = FREE_BLOCK;

    for (i = 0; i < TAILLE_TAS - 1; i++)
    {
        if (*(tas + i) == FREE_BLOCK && count == 1)
        {
            pt2 = tas + i;
            temp2 = tas[i - 1];
            printf("%d\n", temp2);
        }
        if (tas[i] == FREE_BLOCK && count == 0)
        {
            pt1 = tas + i;
            temp1 = tas[i - 1];
            printf("%d\n", temp1);
            count = 1;
        }
    }
    if (pt2 > pt1)
    {
        *pt2 = INIT_VAL;
        libre = pt1 - 1 - tas;
        *(pt2 - 1) = INIT_VAL;
    }
    else
    {
        *pt1 = INIT_VAL;
        libre = pt2 - 1 - tas;
        *(pt1 - 1) = INIT_VAL;
    }

    count = 0;

    return pt - 1;
}

char *print_malloc(void)
{
    int i;
    for (i = 0; i < TAILLE_TAS - 1; i++)
    {
        if (isgraph(tas[i]))
        {
            printf("%c", tas[i]);
            if (iscntrl(tas[i + 1]))
                printf("\\0");
        }
        if (isspace(tas[i]))
        {
            printf("%c", tas[i]);
            if (iscntrl(tas[i + 1]))
                printf("\\0");
        }
        else
            printf(".");
    }
    printf("\n");
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
