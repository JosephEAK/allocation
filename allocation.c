#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void free_heap()
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
        if (tmp->filled == FREE_BLOCK && tmp->size >= SIZE_HEAP)
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
    if (tmp->next->filled == FREE_BLOCK)
    {
        merge_right(tmp);
    }
    if (tmp->prev->filled == FREE_BLOCK)
    {
        merge_right(tmp->prev);
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
            new_free_->next = tmp->next;
            tmp->next = new_free_;
            new_free_->prev = tmp;

            defragmentation(new_free_);

            return tmp->ptr;
        }
    }

    return NULL;
}

void print_heap(void)
{
}