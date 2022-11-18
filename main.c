#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allocation.h"

int main(void)
{
    char *p0 = NULL, *p1 = NULL, *p2 = NULL, *p3 = NULL;

    init_tas();

    p0 = (char *)tas_malloc(10);
    strcpy(p0, "tp 1");
    print_tas();

    p1 = (char *)tas_malloc(9);
    strcpy(p1, "love");
    print_tas();

    p2 = (char *)tas_malloc(15);
    strcpy(p2, "pigeon");
    print_tas();

    p3 = (char *)tas_malloc(15);
    strcpy(p3, "8h$ mh8");
    print_tas();

    return 0;
}