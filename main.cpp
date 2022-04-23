#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <List.h>


typedef struct Phash_table
{
    size_t capacity = 0;
    size_t cur_size = 0;
    struct Plist* hash_el;
    
}Phash_table;


static const size_t Hash_table_capacity = 10;

int hash_table_con(Phash_table* hash_table)
{
    assert(hash_table != nullptr);

    hash_table->cur_size = 0;
    hash_table->capacity = Hash_table_capacity;
    hash_table->hash_el = (struct Plist*)calloc(Hash_table_capacity, sizeof(Plist));

    return 0;
}


int main()
{

}