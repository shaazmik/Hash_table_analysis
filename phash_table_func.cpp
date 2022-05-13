#include "./phash_table.h"


size_t phash_table_verificator(Phash_table* hash_table)
{
    assert(hash_table != nullptr);

    if (hash_table->num_of_el < 0)
    {
        hash_table->error = WRONG_CURRENT_SIZE;
    }
    else if (hash_table->capacity < 0)
    {
        hash_table->error = WRONG_CAPACITY;
    }
    else if (hash_table->left_canary != Phash_canarias)
    {
        hash_table->error = LEFT_CANARY_DEAD;
    }
    else if (hash_table->left_canary != Phash_canarias)
    {
        hash_table->error = RIGHT_CANARY_DEAD;
    }
    return hash_table->error;
}


void phash_table_dump(Phash_table* hash_table)
{
    assert(hash_table != nullptr);

    FILE* log = fopen("log_hash.txt", "w+");

    if (log == nullptr)
    {
        printf("ERROR: LOG FILE NOT FOUND\n");
        abort();
    }

    switch (hash_table->error)
    {
    case (NULLPTR):
        fprintf(log, "ERROR: NULLPTR\n");
        break;

    case (WRONG_CAPACITY):
        fprintf(log, "ERROR: WRONG_CAPACITY\n");
        break;

    case (WRONG_CURRENT_SIZE):
        fprintf(log, "ERROR: WRONT_CURRENT_SIZE\n");
        break;

    case (LEFT_CANARY_DEAD):
        fprintf(log, "ERROR: LEFT_CANARY_DEAD\n"
                     "CANARY VALUE:%lld\n", hash_table->left_canary);
        break;

    case (RIGHT_CANARY_DEAD):
        fprintf(log, "ERROR: RIGHT_CANARY_DEAD\n"
                     "CANARY VALUE:%lld\n", hash_table->right_canary);
        break;
        
    default:
        break;
    }

    fclose(log);
}

int phash_table_con(Phash_table* hash_table)
{
    assert(hash_table != nullptr);

    hash_table->num_of_el    = 0;
    hash_table->capacity     = Hash_table_capacity;
    hash_table->left_canary  = Phash_canarias;
    hash_table->right_canary = Phash_canarias;
    hash_table->hash_list    = (struct Plist*)calloc(Hash_table_capacity, sizeof(Plist));

    hash_table->hash_func = hash_CRC32; 

    for (int i = 0; i < Hash_table_capacity; i++)
    {
        plist_constructor(hash_table->hash_list + i, List_capacity);
    }

    #ifdef PROTECTION

    VERIFICATION_PHASH(hash_table);

    #endif

    return 0;
}

int phash_table_des(Phash_table* hash_table)
{

    #ifdef PROTECTION

    VERIFICATION_PHASH(hash_table);

    #endif

    hash_table->num_of_el    = 0;
    hash_table->capacity     = 0;
    hash_table->left_canary  = Otrava;
    hash_table->right_canary = Otrava;

    for (int i = 0; i < Hash_table_capacity; i++)
    {
        plist_destructor(hash_table->hash_list + i); 
    }

    return 0;
}



size_t find_file_size(FILE* in)
{
    assert(in != nullptr);

    int size_of_file = 0;

    fseek (in, 0, SEEK_END);
    size_of_file = ftell (in);
    fseek (in, 0, SEEK_SET);

    return (size_of_file);
}


char* fill_words_text(size_t file_size, FILE* in)
{
    assert(in != nullptr);

    char* text_array = (char*)calloc(file_size + 1, sizeof(char));

    if (text_array)
    {
        fread(text_array, sizeof(char), file_size, in);
        *(text_array + file_size) = '\0';
        fseek(in, 0, SEEK_SET);
    }
    else
    {
        fseek(in, 0, SEEK_SET);
        return nullptr;
    }

    char* start_text = text_array;

    while (*start_text != '\0')
    {
        if ((*start_text == '.') || (*start_text == '\n') || (*start_text == '?') ||
           ( *start_text == '!') || (*start_text == '\r') || (*start_text == ',') ||
           ( *start_text == '"') || (*start_text == '\''))
        {
            *start_text = ' ';
        }

        start_text++;
    }

    return text_array;
}

int phash_table_input_file(Phash_table* hash_table, FILE* in)
{
    assert(hash_table != nullptr);
    assert(in         != nullptr);

    size_t file_size  = find_file_size(in);
    char*  text_array = fill_words_text(file_size, in);
    char*  word_start = text_array;
    char*  word_end   = text_array;
    int    flag       = 0;

    while ((word_end = strchr(word_start, ' ')) != nullptr)
    {
        if (*word_start != ' ')
        {
            *word_end = '\0';
            phash_table_find_el(hash_table, word_start, &flag);
            phash_table_find_el(hash_table, word_start, &flag);
            phash_table_find_el(hash_table, word_start, &flag);
            phash_table_find_el(hash_table, word_start, &flag);
            phash_table_find_el(hash_table, word_start, &flag);
            if (flag == 0)
                phash_table_insert_el(hash_table, word_start);
            word_start = word_end + 1;
        }
        else
        {
            word_start++;
        }
    }
    phash_table_find_el(hash_table, word_start, &flag);
    if (flag == 0) 
        phash_table_insert_el(hash_table, word_start);

    return (*word_start);
}

int phash_table_insert_el(Phash_table* hash_table, char* word)
{
    size_t hash = hash_table->hash_func(word);

    size_t offset = hash % hash_table->capacity;
    
    hash_table->num_of_el++;

    plist_insert_last(hash_table->hash_list + offset, word);

    return 0;
}

int phash_table_input_func(Phash_table* hash_table, size_t(*hash_func)(char* word))
{
    hash_table->hash_func = hash_func;

    return 0;
}


struct Plist* phash_table_find_el(Phash_table* hash_table, char* word, int* item_num_of_list)
{
    size_t offset = hash_table->hash_func(word) % hash_table->capacity;

    __m128i string = _mm_loadu_si128 ((__m128i *)word);
    __m128i listStr {};

    int flag = 0;

    for (int i = 1; (hash_table->hash_list[offset].size >= i) && (flag == 0) ; i++)
    {
        listStr = _mm_loadu_si128 ((__m128i *)hash_table->hash_list[offset].data[i].value);

        int cmp = _mm_cmpestri (string, strlen (word) + 1, listStr, hash_table->hash_list[offset].data[i].len_str + 1, _SIDD_CMP_EQUAL_EACH | _SIDD_CMP_EQUAL_ORDERED | _SIDD_UBYTE_OPS); // | _SIDD_NEGATIVE_POLARITY);

        if (cmp == 0) 
            flag = i;
    }

    *item_num_of_list = flag;

    return hash_table->hash_list + offset;
}


void phash_table_print_element(Phash_table* hash_table, size_t item_number)
{
    for (int i = 1; i <= hash_table->hash_list[item_number].size; i++)
    {
        printf("element: %s\n", hash_table->hash_list[item_number].data[i].value);
    }

    return;
}