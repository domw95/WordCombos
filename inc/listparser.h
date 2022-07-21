#ifndef LISTPARSER_H
#define LISTPARSER_H

#include <stdbool.h>
#include <stdint.h>



struct Wordlist{
    struct Word **words;
    int length;
};

struct Word{
    char *str;
    uint64_t integer;
    int pos;
    struct Wordlist anagrams;
};


// Struct to hold results of trying to find all combinations
struct Results{
    struct Wordlist *list;
    int total;
};

// struct for holding info about words
struct Words{
    struct Wordlist unique; // words with anagrams removed
    struct Wordlist all;    // all words including anagrams
    int length;
    int count;
    uint64_t *integers;  // list of integer representation of unique to speed up processing
    int target_cnt;

    char *sourcefile;   // file to generate list from
    // stats
    int total_words;
    int total_words_length;
    int total_words_all;
    int total_words_unique;
    
    // results
    struct Results results;
};


// parses the list given in sourcefile, for given length
bool parse_wordlist(struct Words *words, bool remove_anagrams);

// Converts a word to integer representation where bit0 = a, bit1 = b etc
uint64_t word_to_int(const char *word);

bool append_to_list(struct Word *word, struct Wordlist *list);

bool append_to_results(struct Wordlist list, struct Results *results);

void list_anagrams(struct Wordlist *list);


#endif