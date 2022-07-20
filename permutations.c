#include "permutations.h"
#include <stdlib.h>
#include <stdio.h>

bool init_permu(struct Permu *permu, int width, int length){
    permu->length = length;
    permu->width = width;
    permu->pos = malloc(width*sizeof(int));
    permu->limits = malloc(width*sizeof(int));
    for (int i=0; i<width; i++){
        permu->pos[i] = 0;
        permu->limits[i] = length;
    }
    permu->pos[width-1] = -1;
    permu->ind = width-1;
    
}

bool next_permu(struct Permu *permu){
    // increment position at current index
    permu->pos[permu->ind] ++;
    // check if position has reached limit
    if (permu->pos[permu->ind] == permu->limits[permu->ind]){
        // set back to zero and step up
        permu->pos[permu->ind] = 0;
        permu->ind --;
        if (permu->ind == -1){
            // reached end
            return false;
        }
        return next_permu(permu);
    }
    permu->ind = permu->width-1;
    return true;
}

void list_permus(struct Permu *permu){
    while(next_permu(permu)){
        // print out current permu positions
        printf("[");
        for (int i=0; i<permu->width-1; i++){
            printf("%d,",permu->pos[i]);
        }
        printf("%d]\n",permu->pos[permu->width-1]);
    }   
}