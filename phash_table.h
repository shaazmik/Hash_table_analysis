#ifndef PHASH_TABLE_H
#define PHASH_TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "./List/List.h"

#define PROTECTION

//===============================================

enum Phash_table_errors
{
    NULLPTR            = -1,
    OK_HASH_TABLE      =  16,
    WRONG_CAPACITY     =  17,
    WRONG_CURRENT_SIZE =  18,
    LEFT_CANARY_DEAD   =  19,
    RIGHT_CANARY_DEAD  =  20,
};


const size_t Hash_table_capacity       = 100;
const size_t List_capacity             = 10;
const size_t Phash_canarias            = 1337;

//===============================================

#define VERIFICATION_PHASH(phash_table)                         \
{                                                               \
    assert(phash_table != nullptr);                             \
                                                                \
    if (phash_table_verificator(phash_table) != OK_HASH_TABLE)  \
    {                                                           \
        fprintf(stderr, "ERROR CHECK <log.txt>\n");             \
        phash_table_dump(phash_table);                          \
        fprintf(stderr, "ABORTED ON %u\n", __LINE__);           \
        abort();                                                \
    }                                                           \
}                                                               \

//===============================================

typedef struct Phash_table
{
    long long left_canary;

    size_t capacity = 0;
    size_t num_of_el = 0;

    struct Plist* hash_list;

    size_t (*hash_func)(char*);

    size_t error = OK_HASH_TABLE;

    long long right_canary;
}Phash_table;

//===============================================


size_t phash_table_verificator(Phash_table* hash_table);

void phash_table_dump(Phash_table* hash_table);

int phash_table_con(Phash_table* hash_table);

int phash_table_des(Phash_table* hash_table);

size_t find_file_size(FILE* in);

char* fill_words_text(size_t file_size, FILE* in);

int phash_table_input_file(Phash_table* hash_table, FILE* in);

int phash_table_insert_el(Phash_table* hash_table, char* word);



size_t hash_first(char* str);

#endif