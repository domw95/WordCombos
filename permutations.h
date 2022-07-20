#ifndef PERMUTATIONS_H
#define PERMUTATIONS_H

#include <stdbool.h>

struct Permu{
    int length;
    int width;
    int *pos;
    int ind;
    int *limits;
};

bool init_permu(struct Permu *permu, int width, int length);

bool next_permu(struct Permu *permu);

void list_permus(struct Permu *permu);

#endif