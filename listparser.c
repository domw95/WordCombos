#include <string.h>
#include <stdio.h>

#include "listparser.h"
#include "libpopcnt.h"

#define MAXLINELENGTH 256
// parses the list given in sourcefile, for given length
bool parse_wordlist(struct Words *words, bool remove_anagrams){
    // open source file for reading
    FILE *file = fopen(words->sourcefile, "r");
    // reset words struct
    words->total_words = 0;
    words->total_words_length = 0;
    words->total_words_all = 0;
    words->total_words_unique = 0;

    words->unique.words = NULL;
    words->all.words = NULL;

    words->unique.length = 0;
    words->all.length = 0;

    words->results.total = 0;
    words->results.list = NULL;

    // length to store a word
    int string_length = words->length + 1;

    // go through each line, finding words of correct length
    char wordstr[MAXLINELENGTH];
    while(fgets(wordstr, MAXLINELENGTH, file)){
        words->total_words ++;
        // remove \r and \n from string
        wordstr[strcspn(wordstr, "\n\r")] = '\0';
        // check length
        if (strlen(wordstr) == words->length){
            words->total_words_length ++;
            // convert to integer
            uint64_t wordint = word_to_int(wordstr);
            // check if all letters are unique
            if (popcnt64(wordint) == words->length){
                words->total_words_all++;
                // create a word struct and assign string to it
                struct Word *word = malloc(sizeof(struct Word));
                word->str = malloc(string_length*sizeof(char));
                word->integer = wordint;
                word->pos = words->total_words_all-1;
                word->anagrams.words = NULL;
                word->anagrams.length = 0;
                strcpy(word->str,wordstr);

                // add word to list all
                append_to_list(word, &words->all);
                
                // check if word is anagram of word already in unique list
                bool anagram = false;
                if (remove_anagrams){
                    for (int i=0; i<words->unique.length; i++){
                        if (popcnt64(wordint | words->unique.words[i]->integer) == words->length){
                            // all the letters are the same
                            anagram = true;
                            // add this word to anagrams of other word
                            append_to_list(word, &words->unique.words[i]->anagrams);
                        }
                    }
                }
                if (!anagram){
                    words->total_words_unique ++;
                    // add word to unique list
                    append_to_list(word, &words->unique);
                }
            }

        }
    }
    return true;
}

uint64_t word_to_int(const char *word){
    int i = 0;
    uint32_t intword = 0;
    while(word[i]){
        intword |= 1 << ((uint8_t)word[i]-97);
        i++;
    }
    return intword;
}

bool append_to_list(struct Word *word, struct Wordlist *list){
    // increment length
    list->length ++;
    struct Word **ptr = realloc(list->words, list->length*sizeof(struct Word *));
    if (ptr == NULL){
        printf("Error appending to list\n");
        return false;
    }
    list->words = ptr;
    list->words[list->length-1] = word;
    return true;

}

bool append_to_results(struct Wordlist list, struct Results *results){
    results->total ++;
    struct Wordlist *ptr = realloc(results->list, results->total*sizeof(struct Wordlist));
    if (ptr == NULL){
        printf("Error appending to results\n");
        return false;
    }
    results->list = ptr;
    results->list[results->total-1] = list;
}

void list_anagrams(struct Wordlist *list){
    for (int i=0; i<list->length; i++){
        if (list->words[i]->anagrams.length){
            printf("%s : ", list->words[i]->str);
            for (int j=0; j<list->words[i]->anagrams.length; j++){
                printf("%s ", list->words[i]->anagrams.words[j]->str);
            }
            printf("\n");
        }
        
    }
}