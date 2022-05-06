#define $ fprintf(stderr, ">>> %d:\n", __LINE__);

#include "./phash_table.h"



size_t hash_first(char* str)
{
    return str[0];
}


int main()
{

    Phash_table hash_table = {};

    phash_table_con(&hash_table);

    FILE* in = fopen("test.txt", "rb");
    
    phash_table_input_file(&hash_table, in);

    printf("%s and %s\n",hash_table.hash_list[97].data[1].value, hash_table.hash_list[97].data[2].value);
    phash_table_des(&hash_table);

    fclose(in);
}