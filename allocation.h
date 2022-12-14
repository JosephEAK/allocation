#ifndef FONCTION_HEADER
#define FONCTION_HEADER

#include "linked_list_function.h"

#define TAILLE_TAS 128
#define FREE_BLOCK -1
#define INIT_VAL 0
#define MEMORY_FULL -1

int libre;
int count;

char tas[TAILLE_TAS];

void init_tas(void);
char *tas_malloc(unsigned int size);
void print_tas(void);
void tas_free(char *ptr);
char *first_fit(unsigned int size);
int find_new_libre(char *ptr);
int get_index_on_tas(char *ptr);
char get_block_size(char *ptr);

char *(*strategy)(unsigned int size); // pointer function
void set_strategy(char *(*strat)(unsigned int size));
char *first_fit_(unsigned int size);
char *best_fit_(unsigned int size);

#endif
