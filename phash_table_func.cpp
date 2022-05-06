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
    hash_table->hash_list = (struct Plist*)calloc(Hash_table_capacity, sizeof(Plist));

    hash_table->hash_func = hash_first; //HASH DECLARATION

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

    //TODO MAKE STRING WITH PUNCTUATION MARK MAYBE WILL BE FASTER!!!

    while (*start_text != '\0')
    {
        if ((*start_text == '.') || (*start_text == '\n') || (*start_text == '?') ||
           ( *start_text == '!') || (*start_text == '\r') || (*start_text == ','))
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

    size_t file_size = find_file_size(in);
    char* text_array = fill_words_text(file_size, in);
    char* word_start = text_array;
    char* word_end   = text_array;

    while ((word_end = strchr(word_start, ' ')) != nullptr)
    {
        if (*word_start != ' ' && *word_start != '\r' && *word_start != '\n')
        {
            //TODO redaction for punctuation marks
            *word_end = '\0';
            phash_table_insert_el(hash_table, word_start);
            word_start = word_end + 1;
        }
        else
        {
            word_start++;
        }
    }

    phash_table_insert_el(hash_table, word_start);

    return (*word_start);
}

int phash_table_insert_el(Phash_table* hash_table, char* word)
{
    hash_table->num_of_el += 1;
    int offset = hash_table->hash_func(word) % hash_table->capacity;
    
    plist_insert_last(hash_table->hash_list + offset, word);

    return 0;
}