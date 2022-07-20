#include "combinations.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool init_combo(struct Combo *combo, int length, int width){
    combo->length = length;
    combo->width = width;
    combo->pos = malloc(width*sizeof(int));
    combo->ind = 0;
    // start at position -1 so next_combo behaves correctly
    for (int i=0; i<width; i++){
        combo->pos[i] = -1;
    }
    combo->found = malloc(1*sizeof(int *));
    combo->total = 0;
}

bool next_combo(struct Combo *combo){
    // increment position at current index
    combo->pos[combo->ind] ++;
    // check if overrun/ end position would overrun
    if (combo->pos[combo->ind] == combo->length-(combo->width-combo->ind-1)){
        // go back a step
        combo->ind --;
        // check if reached end
        if (combo->ind == -1){
            return false;
        }
        // otherwise increment at this index
        return next_combo(combo);
    }
    return true;
}

bool step_combo(struct Combo *combo){
    // increment position index
    combo->ind ++;
    // check if reached end
    if (combo->ind == combo->width){
        combo->ind --;      
        // reached end of tree
        return true;
    } else {
        // otherwise set valid position at current index
        combo->pos[combo->ind] = combo->pos[combo->ind-1] + 1;
        return false;
    }
}

bool list_combos(struct Combo *combo){
    while(next_combo(combo)){
        // keep stepping unti reached end
        while(!step_combo(combo)){}
        // print out current positions
        printf("[");
        for (int i=0; i<combo->width-1; i++){
            printf("%d,",combo->pos[i]);
        }
        printf("%d]\n", combo->pos[combo->width-1]);
    }
}

bool find_combos(struct Combo *combo, combocallback validate, combocallback found){
    // keep getting next combination
    while(next_combo(combo)){
        // check progress
        if (combo->ind == 0){
            float progress = 100*(float)combo->pos[combo->ind]/combo->length;
            printf("\rChecked %.2f%%, Found %d\r", progress, combo->total);
        }
        // Keep calling validation function and stepping 
        while ((*validate)(combo)){
            // step to next index
            if(step_combo(combo)){
                // reached end, record combo and go to next
                combo->total ++;
                // extend found array
                int** ptr = realloc(combo->found,combo->total*sizeof(int *));
                if (ptr == NULL){
                    printf("Failed to reallocate combo.found\n");
                    return false;
                }
                combo->found = ptr;
                // allocate memory for combo
                combo->found[combo->total-1] = malloc(combo->width*sizeof(int));
                if (combo->found[combo->total-1] == NULL){
                    printf("Failed to allocate found array\n");
                    return false;
                }
                // copy current position to found
                memcpy(combo->found[combo->total-1], combo->pos, combo->width*sizeof(int));                
                // call found callback
                (*found)(combo);
                break;
            }
        }
    }
    printf("\rChecked %.2f%%, Found %d\r", 100.0, combo->total);
}