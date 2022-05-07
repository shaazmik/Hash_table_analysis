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

