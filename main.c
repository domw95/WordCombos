#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>


#include "listparser.h"
#include "combinations.h"
#include "permutations.h"
#include "libpopcnt.h"

#define REMOVE_ANAGRAMS true
#define PRINT_FOUND false


// Create words struct for managing word list and options
struct Words words = {.length = 0, .count = 0, .list = NULL, .mode = 0, .sourcefile = NULL};

// Combo struct for tracking bruteforce checking
struct Combo combo;

// func for validating using integer popcnt method
bool validate_using_popcnt(struct Combo *combo);

bool found_combo_callback(struct Combo *combo);

int main(int argc, char **argv)
{
    
    int option_index = 0;

    while ((option_index = getopt(argc, argv, "l:c:s:m:")) != -1)
    {
        switch (option_index)
        {
        case 'l':
            words.length = atoi(optarg);
            break;
        case 'c':
            words.count = atoi(optarg);
            break;
        case 's':
            words.sourcefile = optarg;
            break;
        case 'm':
            words.mode = atoi(optarg);
            break;
        default:
            printf("Option incorrect\n");
            return 1;
        }
    }

    // check required options have been submitted
    if (!words.count){
        printf("Word count -c option is required\n");
        return 2;
    } else if (!words.length){
        printf("Word length -l option is required\n");
        return 3;
    } else if (!words.sourcefile){
        printf("Sourcefile -s option is required\n");
        return 4;
    }

    // parse wordlist
    if(!parse_wordlist(&words, REMOVE_ANAGRAMS)){
        printf("Failed to parse wordlist\n");
        return 5;
    }
    printf("%d words in list\n", words.total_words);
    printf("%d of length %d\n", words.total_words_length, words.length);
    printf("%d are unique\n", words.total_words_unique);
    if (REMOVE_ANAGRAMS){
        printf("%d after removing anagrams\n", words.total_words_wo_anagrams);
    }

    // list_anagrams(&words);

    // initialise and check combo
    init_combo(&combo, words.total, words.count);
    combo.words = &words;
    // list_combos(&combo);

    find_combos(&combo, validate_using_popcnt,found_combo_callback);

    // generate results from combos and anagrams
    words.results = malloc(combo.total*sizeof(char *));
    words.nresults = combo.total;
    for (int i=0; i<combo.total; i++){
        words.results[i] = malloc(combo.width*sizeof(char *));
        for (int j=0; j<combo.width; j++){
            words.results[i][j] = words.list[combo.found[i][j]];
        }
    }

    // for (int i=0; i<words.nresults; i++){
    //     for (int j=0; j<words.count; j++){
    //         printf("%s ", words.results[i][j]);
    //     }
    //     printf("\n");
    // }
    printf("\nSubstituing in anagrams\n");
    // create more results from anagrams
    // need to do permutations for this
    for (int i=0; i<combo.total; i++){
        struct Permu permu;
        init_permu(&permu, combo.width, 0);
        for (int j=0; j<combo.width; j++){
            permu.limits[j] = words.nanagrams[combo.found[i][j]] + 1;            
        }
        // remove all 0 permutation
        next_permu(&permu);
        // go through rest

        // printf("\n");
        while(next_permu(&permu)){
            char **result = malloc(combo.width*sizeof(char *));
            for(int j=0; j<combo.width; j++){                
                if (permu.pos[j]){
                    // place anagram here
                    result[j] = words.anagrams[combo.found[i][j]][permu.pos[j]-1];
                } else {
                    // copy original word
                    result[j] = words.list[combo.found[i][j]];
                }

            }
            // increase size of results
            words.nresults ++;
            char ***ptr = realloc(words.results, words.nresults*sizeof(char *));
            if (ptr == NULL){
                printf("Failed to reallocate words.results");
                return 7;
            }
            words.results = ptr;
            words.results[words.nresults-1] = result;
        }
        free(permu.pos);
        free(permu.limits);
    }
    printf("Total found = %d\n",words.nresults);
}

bool validate_using_popcnt(struct Combo *combo){
    // how many bits should be said after words are bitwise orred together
    int target_cnt = combo->words->length*2;
    // check each of the previous items in combo against current
    for (int i=0; i<combo->ind; i++){
        // bitwise or then popcnt and compare to target count
        if (popcnt64(combo->words->integer[combo->pos[i]] | 
            combo->words->integer[combo->pos[combo->ind]]) != target_cnt){
                return false;
        }
    }    
    return true;
}

bool found_combo_callback(struct Combo *combo){
    // // print words
    if(PRINT_FOUND){
        for (int i=0; i<combo->width; i++){
            printf("%s ", combo->words->list[combo->found[combo->total-1][i]]);
        }
        printf("\n");
    }    
}