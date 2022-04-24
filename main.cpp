#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "./List/List.h"


enum Phash_table_errors
{
    OK_HASH_TABLE      =  1,
    NULLPTR            = -1,
    WRONG_CAPACITY     = 0x11AA,
    WRONG_CURRENT_SIZE = 0x11BB,
    LEFT_CANOREA_DEAD  = 0x1F,
    RIGHT_CANOREA_DEAD = 0x2F,
};

typedef struct Phash_table
{
    size_t capacity = 0;
    size_t cur_size = 0;
    struct Plist* hash_el;
    

    size_t error         = OK_HASH_TABLE;

    size_t left_canorea  = 1337;
    size_t right_canorea = 1338;

}Phash_table;


static const size_t Hash_table_capacity = 100;
static const size_t List_capacity       = 10;

int verificator(Phash_table* hash_table)
{
    if (hash_table->cur_size < 0)
    {
        
    }
}


int hash_table_con(Phash_table* hash_table)
{
    assert(hash_table != nullptr);

    hash_table->cur_size = 0;
    hash_table->capacity = Hash_table_capacity;
    hash_table->hash_el = (struct Plist*)calloc(Hash_table_capacity, sizeof(Plist));

    for (int i = 0; i < Hash_table_capacity; i++)
    {
        plist_constructor(hash_table->hash_el, List_capacity);
    }

    return 0;
}

int hash_table_des(Phash_table* hash_table)
{
    hash_table->cur_size = 0;
    hash_table->capacity = 0;
    
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