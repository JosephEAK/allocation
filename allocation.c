#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "allocation.h"

void *heap;
buffer_tracker *head_tracker;

void *get_heap()
{
    return heap;
}

buffer_tracker *get_buffer_tracker()
{
    return head_tracker;
}

void init_heap(void)
{
    heap = malloc(SIZE_HEAP);
    head_tracker = d_ll_get_new_elem(heap, SIZE_HEAP, FREE_BLOCK);
}

void free_heap(void)
{
    free(heap);
    d_ll_free(head_tracker);
}

buffer_tracker *first_fit(unsigned int size)
{
    buffer_tracker *tmp;
    tmp = head_tracker;

    while (tmp != NULL)
    {
        if (tmp->filled == FREE_BLOCK && tmp->size >= size)
        {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

void merge_right(buffer_tracker *tmp)
{
    buffer_tracker *tmp_next;
    tmp_next = tmp->next;
    tmp->size += tmp->next->size;
    tmp->next = tmp->next->next;
    free(tmp_next);
}

void defragmentation(buffer_tracker *tmp)
{

    if (tmp->next != NULL)
    {
        if (tmp->next->filled == FREE_BLOCK)
        {
            merge_right(tmp);
        }
    }

    if (tmp->prev != NULL)
    {
        if (tmp->prev->filled == FREE_BLOCK)
        {
            merge_right(tmp->prev);
        }
    }
}

void *allocate_memory(unsigned int size)
{
    if (!size)
        return NULL;

    buffer_tracker *tmp;
    tmp = first_fit(size);

    if (tmp)
    {
        if (size == tmp->size)
        {
            tmp->filled = ALLOCATED;

            return tmp->ptr;
        }
        else
        {
            unsigned int left_size = tmp->size - size;
            tmp->size = size;
            tmp->filled = ALLOCATED;

            buffer_tracker *new_free_;
            new_free_ = d_ll_get_new_elem((char *)tmp->ptr + size, left_size, FREE_BLOCK);

            d_ll_add_after(tmp, new_free_);

            defragmentation(new_free_);

            return tmp->ptr;
        }
    }
    return NULL;
}

void print_heap(void)
{
    /*int i, j, col = 15, line = 20;
    buffer_tracker *tmp;
    tmp = heap;

    while (tmp != NULL)
    {
        printf("%d");

        tmp = tmp->next;
    }

    printf("---------------------------------------------------------------\n\n");*/
}

void print_head_tracker(void)
{

    double_linked_list *tmp;
    tmp = head_tracker;
    while (tmp != NULL)
    {
        printf("%ld %d -> ", (char *)tmp->ptr - (char *)heap, tmp->size);

        tmp = tmp->next;
    }
    printf("\n");
}

void ecrire_date_heure_adresse(void *ptr, int size)
{
    // Ouverture du fichier en mode écriture
    FILE *fp = fopen("allocation.txt", "w");

    if (fp == NULL)
    {
        printf("Impossible d'ouvrir le fichier.\n");
        return;
    }

    // Obtention de la date et de l'heure actuelles
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    strftime(s, sizeof(s), "%c", tm);

    // Écriture de la date, de l'heure, de l'adresse du pointeur et de la taille dans le fichier
    fprintf(fp, "Date et heure actuelles: %s\n", s);
    fprintf(fp, "Adresse du pointeur: %p\n", ptr);
    fprintf(fp, "Taille allouée: %d octets\n", size);

    // Fermeture du fichier
    fclose(fp);
}

void free_memory(char *ptr)
{
    double_linked_list *tmp;
    tmp = head_tracker;
    while (tmp != NULL)
    {
        if (tmp->ptr == ptr)
        {
            tmp->filled = FREE_BLOCK;
            defragmentation(tmp);
        }

        tmp = tmp->next;
    }
}