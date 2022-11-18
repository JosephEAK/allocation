#ifndef FONCTION_HEADER
#define FONCTION_HEADER

#define TAILLE_TAS 128
#define FREE_BLOCK -1
#define INIT_VAL 0

char tas[TAILLE_TAS] = {TAILLE_TAS - 1, -1};
int libre = 0;
int count = 0;

void init_tas(void);
char *tas_malloc(unsigned int taille);
char *print_malloc(void);
char *tas_free(char *pt);
void print_tas(void);

#endif
