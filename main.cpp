#define $ fprintf(stderr, ">>> %d:\n", __LINE__);

#include "./phash_table.h"



size_t hash_first_ASCII(char* str)
{
    return str[0];
}

size_t hash_one(char* str)
{
    return 1;
}

size_t hash_len_str(char* str)
{
    return strlen(str);
}


int main()
{

    Phash_table hash_table = {};

    phash_table_con(&hash_table);

    FILE* in = fopen("War_and_Peace.txt", "rb");
    
//    phash_table_input_func(&hash_table, hash_one);

    phash_table_input_file(&hash_table, in);

    printf("size of el:%lu\n", hash_table.num_of_el);

//    printf("%s and %s\n",hash_table.hash_list[97].data[1].value, hash_table.hash_list[97].data[2].value);
    phash_table_des(&hash_table);

    fclose(in);
}