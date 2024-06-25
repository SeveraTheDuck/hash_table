#include "common.h"



//-----------------------------------------------------------------------------
// Test functions implementation
//-----------------------------------------------------------------------------

hash_table_t*
FillHashTable (const char* const filename,
               const size_t buckets_number,
               hash_function h_func)
{
    assert (filename);
    assert (h_func);

    hash_table_t* const table = HashTableConstructor (buckets_number, h_func);
    assert (table);

    text_separation* text_sep = SeparateTextFile (filename, Separator);
    assert (text_sep);

    const size_t words_number = text_sep->strings_number;
    hash_table_key* cur_key = NULL;

    for (size_t i = 0; i < words_number; ++i)
    {
        cur_key = (hash_table_key*) text_sep->strings_array[i];
        HashTableInsert (table, cur_key, NULL, KeyCmpFunction);
    }

    text_sep = DestroySeparation (text_sep);

    return table;
}


hash_table_key_cmp_t
KeyCmpFunction (hash_table_key* const key1,
                hash_table_key* const key2)
{
    assert (key1);
    assert (key2);

    const size_t key1_size = key1->key_size;
    const size_t key2_size = key2->key_size;

    if (key1_size < key2_size) return HASH_TABLE_KEY_CMP_LESS;
    if (key1_size > key2_size) return HASH_TABLE_KEY_CMP_GREATER;

    return memcmp (key1->key, key2->key, key1_size);
}


separator_function_status_t
Separator (const char* const symbol)
{
    assert (symbol);

    char c = *symbol;
    if (isalpha (c)) return NOT_SEPARATOR_ELEMENT;
    return SEPARATOR_ELEMENT_NOT_TAKE;
}


hash_function
GetHashFunctionPointer (const size_t hash_function_index)
{
    hash_function functions_array[] =
    {
        HashFunctionZero,
        HashFunctionFirstASCII,
        HashFunctionStringLength,
        HashFunctionSumASCII,
        // HashFunctionRol,
        // HashFunctionRor,
        HashFunctionDjb2,
        HashFunctionCrc32
    };

    if (hash_function_index >= sizeof (functions_array) / sizeof (hash_function))
        return NULL;

    return functions_array[hash_function_index];
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
