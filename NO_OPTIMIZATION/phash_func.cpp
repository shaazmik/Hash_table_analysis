#include "phash_table.h"

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
//2275 //3069 //3650 - intresting elements in hash table
    size_t result = 0;
    while (*str != '\0')
    {
        result += (int)*str;
        str++;
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

void create_statistic(Phash_table* hash_table)
{
    FILE* out = fopen(".//hash_statistics//results.csv", "w+");

    if (out == nullptr)
    {
        printf("Haven't got folder \"hash_statistics\"!!!\n");
        return;
    }

    for (int i = 0; hash_table->capacity > i; i++)
    {
        fprintf(out, "%d;%lu\n", i, hash_table->hash_list[i].size);
    }

    fclose(out);
}