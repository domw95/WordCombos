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

bool find_combos(struct Combo *combo, combocallback validate, combocallback found, combocallback loop){
    // keep getting next combination until it finishes
    while(next_combo(combo)){
        // call loop callback
        (*loop)(combo);
        // Keep calling validation function and stepping 
        while ((*validate)(combo)){
            // step to next index
            if(step_combo(combo)){    
                // reached a valid combo, call found func then go to next combo            
                (*found)(combo);
                break;
            }
        }
    }
}