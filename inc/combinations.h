#ifndef COMBINATIONS_H
#define COMBINATIONS_H

#include <stdbool.h>
#include "listparser.h"



struct Combo{
    int length;
    int width;
    int *pos;
    int ind;
    void *data;
    uint64_t total;
};

// typedef for validation function
typedef bool (*combocallback)();

// Initialise a combination with given width and length
bool init_combo(struct Combo *combo, int length, int width);

// Increments position at index and ensures validity of combination
bool next_combo(struct Combo *combo);

// Increments index of combintation and checks for end point
bool step_combo(struct Combo *combo);

// print all the possible combinations
bool list_combos(struct Combo *combo);

// Find all combinations 
bool find_combos();

bool validate_callback(struct Combo *combo);

void found_callback(struct Combo *combo);

void loop_callback(struct Combo *combo);

#endif