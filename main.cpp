#define $ fprintf(stderr, ">>> %d:\n", __LINE__);

#include "./phash_table.h"



size_t hash_first_ASCII(char* str)
{
    assert (str != nullptr);

    return str[0];
}

size_t hash_one(char* str)
{
    assert(str != nullptr);

    return 1;
}

size_t hash_len_str(char* str)
{
    assert(str != nullptr);

    return strlen(str);
}

size_t hash_sum_of_ASCII(char* str)
{
    assert(str != nullptr);

    size_t result = 0;
    while (*str != '\0')
    {
        result += *str;
    }

    return result;
}


size_t hash_rol(char* str)
{
    assert (str != nullptr);

    int hash = *str;
    str++;

    while (*str != '\0')
    {
        hash = (hash << 1) ^ *str;

        str++;
    }

    return hash;
}


size_t hash_CRC32(char* str)
{
    assert(str != nullptr);

    unsigned int crc = 0xFFFFFFFFUL;

    while (*str != '\0')
    {
        crc = CRCTable[(crc ^ *(str)) & 0xFF] ^ (crc >> 8);
        str++;
    }

    return crc;
}

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