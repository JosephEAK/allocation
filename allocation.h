#ifndef DLL_ALLOCATION
#define DLL_ALLOCATION

#include "linked_list.h"

#define SIZE 128
#define SIZE_HEAP 300
#define FREE_BLOCK -1
#define INIT_VAL 0
#define MEMORY_FULL -1
#define ALLOCATED 1

typedef double_linked_list buffer_tracker;

void init_heap(void);
void *allocate_memory(unsigned int size);
void print_heap(void);
buffer_tracker *get_buffer_tracker();
void *get_heap();
void free_heap(void);
void ecrire_date_heure_adresse(void *ptr, int size);

buffer_tracker *first_fit(unsigned int size);

#endif /* END_DLL_ALLOCATION */
