#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>


#include "listparser.h"
#include "combinations.h"
#include "permutations.h"
#include "libpopcnt.h"

#define REMOVE_ANAGRAMS true
#define PRINT_FOUND false


// Create words struct for managing word lists and results
struct Words words = {.count=0, .length=0, .sourcefile=NULL};

// Combo struct for tracking bruteforce checking
struct Combo combo;

// func for validating using integer popcnt method
bool validate_using_popcnt(struct Combo *combo);
// callbacks for combinations process
bool found_combo_callback(struct Combo *combo);

bool combo_loop_callback(struct Combo *combo);

int main(int argc, char **argv)
{
    // parse command line options
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
            // words.mode = atoi(optarg);
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

    // Print wordlist stats
    printf("%d words in list\n", words.total_words);
    printf("%d of length %d\n", words.total_words_length, words.length);
    printf("%d have no repeating letters\n", words.total_words_all);
    if (REMOVE_ANAGRAMS){
        printf("%d after removing anagrams\n", words.total_words_unique);
    }

    // initialise and check combo
    init_combo(&combo, words.unique.length, words.count);
    combo.data = &words;
    
    // Check all combintations
    printf("Checking combos\n");
    find_combos(&combo, validate_using_popcnt,found_combo_callback, combo_loop_callback);
    printf("Checked %.2f%%, Found %d\n", 100.0, words.results.total);

    // create more results from anagrams
    printf("Substituing in anagrams\n");
    int nresults = words.results.total;

    for (int i=0; i<nresults; i++){
        struct Permu permu;
        init_permu(&permu, combo.width, 0);
        for (int j=0; j<combo.width; j++){
            permu.limits[j] = words.results.list[i].words[j]->anagrams.length +1;
        }
        // remove all 0 permutation
        next_permu(&permu);
        
        // go through rest
        while(next_permu(&permu)){
            struct Wordlist list;
            list.words = NULL;
            list.length = 0;
            for(int j=0; j<combo.width; j++){                
                if (permu.pos[j]){
                    // place anagram here
                    append_to_list(words.results.list[i].words[j]->anagrams.words[permu.pos[j]-1], &list);
                } else {
                    // copy original word
                    append_to_list(words.results.list[i].words[j], &list);
                }
            }
            append_to_results(list, &words.results);
        }
        free(permu.pos);
        free(permu.limits);
    }

    printf("Total = %d\n",words.results.total);

    // save results to file
    char filename[60];
    sprintf(filename, "results/Word_length_%d_combo_%d", words.length, words.count);
    FILE *outfile = fopen(filename, "w");

    for (int i=0; i<words.results.total; i++){
        for (int j=0; j<words.results.list[i].length; j++){
            fputs(words.results.list[i].words[j]->str, outfile);
            fputc(' ', outfile);
        }
        fputc('\n', outfile);
    }
    printf("Results saved to %s\n", filename);

}

// function for validating current combintation of words
bool validate_using_popcnt(struct Combo *combo){
    // get ptr to words stuct
    struct Words *words = ((struct Words *)combo->data);

    // check each of the previous items in combo against current
    for (int i=0; i<combo->ind; i++){
        // bitwise or then popcnt and compare to target count
        if (popcnt64(words->integers[combo->pos[i]]| 
            words->integers[combo->pos[combo->ind]] ) != words->target_cnt){
                return false;
        }
    }    
    return true;
}

// function called when a valid combination has been found
bool found_combo_callback(struct Combo *combo){
    struct Words *words = ((struct Words *)combo->data);
    // reached end, record result and continue
    struct Wordlist list;
    list.words = NULL;
    list.length = 0;

    // fetch words from wordlists
    for (int i=0; i<words->count; i++){
        append_to_list(words->unique.words[combo->pos[i]], &list);
    }
    // add result to results
    append_to_results(list, &words->results);
}

// Function called on every combination loop
bool combo_loop_callback(struct Combo *combo){
    if (combo->ind == 0){
        float progress = 100*(float)combo->pos[combo->ind]/combo->length;
        printf("\rChecked %.2f%%, Found %d\r", progress, ((struct Words *)combo->data)->results.total);
    }
}