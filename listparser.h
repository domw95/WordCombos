#ifndef LISTPARSER_H
#define LISTPARSER_H

#include <stdbool.h>
#include <stdint.h>

// struct for holding info about words
struct Words{
    int length;     // length of words of interest
    int count;      // how many to put together
    char **list;    // list of words
    int total;      // how many words in list
    char **unqiue_list; // list of words with no repeating chars
    int *integer;   // list of integer representations of words
    int mode;       // method to find combos
    char *sourcefile;   // file to generate list from
    char ***anagrams;    // list to string of anagramsf for each found during import
    int *nanagrams;     // number of anagrams for this word
    // stats
    int total_words;
    int total_words_length;
    int total_words_unique;
    int total_words_wo_anagrams;
    // results
    char ***results;
    int nresults;

};

// Struct to hold results of trying to find all combinations
struct Results{
    char **combos;
    int total;
    struct Words *words;
};

// parses the list given in sourcefile, for given length
bool parse_wordlist(struct Words *words, bool remove_anagrams);

// Converts a word to integer representation where bit0 = a, bit1 = b etc
uint64_t word_to_int(const char *word);


void list_anagrams(struct Words *words);

// return all anagrams for given words
// bool get_anagrams();

#endif