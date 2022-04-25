#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "./List/List.h"


enum Phash_table_errors
{
    NULLPTR            = -1,
    OK_HASH_TABLE      =  16,
    WRONG_CAPACITY     =  17,
    WRONG_CURRENT_SIZE =  18,
    SIZE_MORE_CAPACITY =  19,
    LEFT_CANOREA_DEAD  =  20,
    RIGHT_CANOREA_DEAD =  21,
};

typedef struct Phash_table
{
    long long left_canary;

    size_t capacity = 0;
    size_t num_of_el = 0;
    struct Plist* hash_el;

    int (*hash_func)(char*);

    size_t error         = OK_HASH_TABLE;

    long long right_canary;
}Phash_table;


static const size_t Hash_table_capacity = 100;
static const size_t List_capacity       = 10;
static const size_t Canarias            = 1337;

int verificator_phash(Phash_table* hash_table)
{
    assert(hash_table != nullptr);

    if (hash_table->num_of_el < 0)
    {
        hash_table->error = WRONG_CURRENT_SIZE;
        //dump();
    }
    if (hash_table->capacity < 0)
    {
        hash_table->error = WRONG_CAPACITY;
    }
    if (hash_table->left_canary != Canarias)
    {
        hash_table->error = LEFT_CANOREA_DEAD;
    }
    if (hash_table->left_canary != Canarias)
    {
        hash_table->error = RIGHT_CANOREA_DEAD;
    }

    return hash_table->error;
}


void dump(Phash_table* hash_table)
{
    assert(hash_table != nullptr);

    FILE* log = fopen("log.txt", "w+");

    switch (hash_table->error)
    {
    case (NULLPTR):
        /* code */
        break;
    
    default:
        break;
    }
}


int hash_table_con(Phash_table* hash_table)
{
    assert(hash_table != nullptr);

    hash_table->num_of_el    = 0;
    hash_table->capacity     = Hash_table_capacity;
    hash_table->left_canary  = Canarias;
    hash_table->right_canary = Canarias;
    hash_table->hash_el = (struct Plist*)calloc(Hash_table_capacity, sizeof(Plist));

    //PUT HEAR YOUR HASH FUNC


    for (int i = 0; i < Hash_table_capacity; i++)
    {
        plist_constructor(hash_table->hash_el, List_capacity);
    }

    return 0;
}

int hash_table_des(Phash_table* hash_table)
{
    hash_table->num_of_el    = 0;
    hash_table->capacity     = 0;
    hash_table->left_canary  = Otrava;
    hash_table->right_canary = Otrava;

    for (int i = 0; i < Hash_table_capacity; i++)
    {
        plist_destructor(hash_table->hash_el); 
    }

    return 0;
}



int main()
{

    Phash_table hash_table = {};

    hash_table_con(&hash_table);

    hash_table_des(&hash_table);
}