#define $ fprintf(stderr, ">>> %d:\n", __LINE__);


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "./List/List.h"


#define PROTECTION

enum Phash_table_errors
{
    NULLPTR            = -1,
    OK_HASH_TABLE      =  16,
    WRONG_CAPACITY     =  17,
    WRONG_CURRENT_SIZE =  18,
    LEFT_CANARY_DEAD   =  19,
    RIGHT_CANARY_DEAD  =  20,
};


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


static const size_t Hash_table_capacity = 100;
static const size_t List_capacity       = 10;
static const size_t Canarias            = 1337;

size_t phash_table_verificator(Phash_table* hash_table)
{
    assert(hash_table != nullptr);

    if (hash_table->num_of_el < 0)
    {
        hash_table->error = WRONG_CURRENT_SIZE;
    }
    else if (hash_table->capacity < 0)
    {
        hash_table->error = WRONG_CAPACITY;
    }
    else if (hash_table->left_canary != Canarias)
    {
        hash_table->error = LEFT_CANARY_DEAD;
    }
    else if (hash_table->left_canary != Canarias)
    {
        hash_table->error = RIGHT_CANARY_DEAD;
    }

    return hash_table->error;
}


void phash_table_dump(Phash_table* hash_table)
{
    assert(hash_table != nullptr);

    FILE* log = fopen("log.txt", "w+");

    if (log == nullptr)
    {
        printf("ERROR: LOG FILE NOT FOUND\n");
        abort();
    }

    switch (hash_table->error)
    {
    case (NULLPTR):
        fprintf(log, "ERROR: NULLPTR\n");
        break;

    case (WRONG_CAPACITY):
        fprintf(log, "ERROR: WRONG_CAPACITY\n");
        break;

    case (WRONG_CURRENT_SIZE):
        fprintf(log, "ERROR: WRONT_CURRENT_SIZE\n");
        break;

    case (LEFT_CANARY_DEAD):
        fprintf(log, "ERROR: LEFT_CANARY_DEAD\n"
                     "CANARY VALUE:%lld\n", hash_table->left_canary);
        break;

    case (RIGHT_CANARY_DEAD):
        fprintf(log, "ERROR: RIGHT_CANARY_DEAD\n"
                     "CANARY VALUE:%lld\n", hash_table->right_canary);
        break;
        
    default:
        break;
    }

    fclose(log);
}

size_t hash_first(char* str)
{
    return str[0];
}

int phash_table_con(Phash_table* hash_table)
{
    assert(hash_table != nullptr);

    hash_table->num_of_el    = 0;
    hash_table->capacity     = Hash_table_capacity;
    hash_table->left_canary  = Canarias;
    hash_table->right_canary = Canarias;
    hash_table->hash_list = (struct Plist*)calloc(Hash_table_capacity, sizeof(Plist));

    hash_table->hash_func = hash_first; //HASH DECLARATION

    for (int i = 0; i < Hash_table_capacity; i++)
    {
        plist_constructor(hash_table->hash_list + i, List_capacity);
    }

    #ifdef PROTECTION

    VERIFICATION_PHASH(hash_table);

    #endif

    return 0;
}

int phash_table_des(Phash_table* hash_table)
{

    #ifdef PROTECTION

    VERIFICATION_PHASH(hash_table);

    #endif

    hash_table->num_of_el    = 0;
    hash_table->capacity     = 0;
    hash_table->left_canary  = Otrava;
    hash_table->right_canary = Otrava;

    for (int i = 0; i < Hash_table_capacity; i++)
    {
        plist_destructor(hash_table->hash_list + i); 
    }

    return 0;
}


int phash_table_insert_el(Phash_table* hash_table, char* word)
{
    hash_table->num_of_el += 1;
    int offset = hash_table->hash_func(word) % hash_table->capacity;

    plist_insert_last(hash_table->hash_list + offset, 1337);

    return 0;
}

int main()
{

    Phash_table hash_table = {};

    phash_table_con(&hash_table);

    char word[8] = "balumba";

    printf("%d", hash_first(word));

  //  phash_table_insert_el(&hash_table, )

    phash_table_des(&hash_table);
}