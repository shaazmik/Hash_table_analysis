#define $ fprintf(stderr, ">>> %d:\n", __LINE__);
#include "./phash_table.h"


int main()
{

    Phash_table hash_table = {};

    phash_table_con(&hash_table);

    FILE* in = fopen("War_and_Peace.txt", "rb");
    
    phash_table_input_file(&hash_table, in);

//    char str[15] = "see";
//    int num_of_list_el = 0;
//    struct Plist* element  = phash_table_find_el(&hash_table, str, &num_of_list_el);
//    printf("list num: %p and num of list el:%d\n", element, num_of_list_el);
//    printf("element is: %s\n", element->data[num_of_list_el].value);
    
    phash_table_des(&hash_table);

    fclose(in);
}