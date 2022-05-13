#define $ fprintf(stderr, ">>> %d:\n", __LINE__);
#include "./phash_table.h"


int main()
{

    Phash_table hash_table = {};

    phash_table_con(&hash_table);

    FILE* in = fopen("War_and_Peace.txt", "rb");
    
    phash_table_input_func(&hash_table, crc32_asm);   

    phash_table_input_file(&hash_table, in);
    
    printf("number of elements:%ld\n", hash_table.num_of_el);

    phash_table_des(&hash_table);

    fclose(in);
}