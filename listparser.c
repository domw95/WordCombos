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
    words->total_words_unique = 0;
    words->total_words_wo_anagrams = 0;
    words->total = 0;    

    // allocate memory for lists
    words->list = NULL;
    words->anagrams = NULL;
    words->nanagrams = NULL;
    words->unqiue_list = NULL;
    words->integer = NULL;

    // length to store a word
    int string_length = words->length + 1;

    // go through each line, finding words of correct length
    char word[MAXLINELENGTH];
    while(fgets(word, MAXLINELENGTH, file)){
        words->total_words ++;
        // remove \r and \n from string
        word[strcspn(word, "\n\r")] = '\0';
        // check length
        if (strlen(word) == words->length){
            words->total_words_length ++;
            // convert to integer
            uint64_t wordint = word_to_int(word);
            // printf("%s is 0x%08X\n",word,wordint);
            if (popcnt64(wordint) == words->length){
                words->total_words_unique ++;
                // increase unique list size                         
                char **list_ptr = realloc(words->unqiue_list, words->total_words_unique*sizeof(char *));
                if (list_ptr == NULL){
                    printf("Failed to reallocate memory for words.unique_list");
                    return false;
                }
                words->unqiue_list = list_ptr;
                // create char array for word
                char *word_ptr = malloc(string_length * sizeof(char));
                if (word_ptr == NULL){
                    printf("Failed to allocate memory for word\n");
                    return false;
                }
                // add to unique list
                words->unqiue_list[words->total_words_unique-1] = word_ptr;
                strcpy(words->unqiue_list[words->total_words_unique-1], word);

                // check if an anagram has already appeared
                bool anagram = false;
                if (remove_anagrams){
                    for (int i=0; i<words->total; i++){
                        if (popcnt64(wordint | words->integer[i]) == words->length){
                            // All the letters are the same
                            anagram = true;
                            // add this word to list of matching words anagrams
                            // increment number of anagrams for this word
                            words->nanagrams[i] ++;
                            // reallocate memory for longer list                            
                            char **ana_elem_ptr = realloc(words->anagrams[i], words->nanagrams[i]*sizeof(char *));
                            if (ana_elem_ptr == NULL){
                                printf("Failed to reallocate memory for anagram elem\n");
                                return false;
                            }
                            words->anagrams[i] = ana_elem_ptr;
                            words->anagrams[i][words->nanagrams[i]-1] = word_ptr;
                            break;
                        }
                    }
                }
                if (!anagram){
                    words->total_words_wo_anagrams ++;
                    words->total ++;
                    // reallocate memory for integer, list and anagrams                    
                    int *temp_ptr= realloc(words->integer, words->total*sizeof(int));
                    if(temp_ptr == NULL){
                        printf("Failed to reallocate memory for words.integer");
                        return false;
                    }
                    words->integer = temp_ptr;
                    list_ptr = realloc(words->list, words->total*sizeof(char *));
                    if (list_ptr == NULL){
                        printf("Failed to reallocate memory for words.list");
                        return false;
                    }
                    words->list = list_ptr;                    
                    char ***ana_ptr = realloc(words->anagrams, words->total*sizeof(char *));
                    if (list_ptr == NULL){
                        printf("Failed to reallocate memory for words.anagrams");
                        return false;
                    }
                    words->anagrams = ana_ptr;
                    
                    // increase nanagram list size
                    temp_ptr = realloc(words->nanagrams, words->total*sizeof(int));
                    if (temp_ptr == NULL){
                        printf("Failed to reallocate memory for words.nanagrams");
                        return false;
                    }
                    words->nanagrams = temp_ptr;
                    
                    // set number of anagrams to 0
                    words->nanagrams[words->total-1] = 0;
                    // set anagram ptr to null
                    words->anagrams[words->total-1] = NULL;
                    // add word to lists
                    words->integer[words->total-1] = wordint;
                    words->list[words->total-1] = word_ptr;

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

void list_anagrams(struct Words *words){
    for (int i=0; i<words->total; i++){
        if (words->nanagrams[i]){
            printf("%s : ", words->list[i]);
            for (int j=0; j<words->nanagrams[i]; j++){
                printf("%s ", words->anagrams[i][j]);
            }
            printf("\n");
        }
        
    }
}