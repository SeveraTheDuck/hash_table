#include "hash_table.h"
#include <assert.h>



//-----------------------------------------------------------------------------
// Hash table structure
//-----------------------------------------------------------------------------

/**
 * @brief Type for hash table bucket
 */
typedef list_t hash_table_bucket;


struct hash_table
{
    hash_table_bucket** buckets;        ///< array of buckets
    size_t              buckets_num;    ///< number of buckets
    hash_function       h_func;         ///< hash function
    size_t              elem_number;    ///< number of elements
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Static functions prototypes
//-----------------------------------------------------------------------------

/**
 * @brief Get the bucket for the given key
 *
 * @param table Hash table
 * @param key Key to find bucket
 *
 * @retval Pointer to the bucket
 * @retval NULL if bad input error occurred
 *
 * @details If the bucket was not constructed yet, constructs and returns it
 */
static hash_table_bucket*
GetBucket (hash_table_t* const table,
           hash_table_key* const key);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Interface functions implementation
//-----------------------------------------------------------------------------

hash_table_t*
HashTableConstructor (const size_t buckets_number,
                      hash_function h_func)
{
    if (buckets_number == 0 ||
        h_func         == NULL)
        return NULL;

    hash_table_t* table = malloc(sizeof (hash_table_t));
    if (table == NULL) return NULL;

    table->buckets = calloc (buckets_number, sizeof (hash_table_bucket*));
    if (table->buckets == NULL)
        return HashTableDestructor (table);

    table->buckets_num = buckets_number;
    table->h_func      = h_func;
    table->elem_number = 0;

    return table;
}


hash_table_t*
HashTableDestructor (hash_table_t* const table)
{
    if (table == NULL) return NULL;

    const size_t buckets_number = table->buckets_num;
    hash_table_bucket** buckets = table->buckets;

    if (buckets != NULL)
        for (size_t i = 0; i < buckets_number; ++i)
            buckets[i] = ListDestructor (buckets[i]);

    free (buckets);
    free (table);

    return NULL;
}


hash_table_key*
HashTableKeyConstructor (const void* const key_buffer,
                         const size_t key_size)
{
    return ListKeyConstructor (key_buffer, key_size);
}


hash_table_key*
HashTableKeyDestructor (hash_table_key* const key)
{
    return ListKeyDestructor (key);
}


hash_table_value*
HashTableValueConstructor (const void* const value_buffer,
                           const size_t value_size)
{
    return ListValueConstructor (value_buffer, value_size);
}


hash_table_value*
HashTableValueDestructor (hash_table_value* const value)
{
    return ListValueDestructor (value);
}


hash_table_error_status
HashTableInsert (hash_table_t*     const table,
                 hash_table_key*   const key,
                 hash_table_value* const value)
{
    if (table         == NULL ||
        table->h_func == NULL ||
        key           == NULL)
        return HASH_TABLE_ERROR;

    hash_table_bucket* const bucket = GetBucket (table, key);

    if (ListPushBack (bucket, key, value) == LIST_ERROR)
        return HASH_TABLE_ERROR;

    ++table->elem_number;
    return HASH_TABLE_SUCCESS;
}


hash_table_error_status
HashTableDelete (hash_table_t*   const table,
                 hash_table_key* const key,
                 hash_table_key_comparator key_cmp)
{
    if (table         == NULL ||
        table->h_func == NULL ||
        key           == NULL)
        return HASH_TABLE_ERROR;

    hash_table_bucket* const bucket = GetBucket (table, key);
    hash_table_node*   const node   = ListFindNode (bucket, key, key_cmp);

    if (node == NULL) return HASH_TABLE_ERROR;

    if (ListDeleteNode (bucket, node) == LIST_ERROR)
        return HASH_TABLE_ERROR;

    --table->elem_number;
    return HASH_TABLE_SUCCESS;
}


hash_table_node*
HashTableFind (hash_table_t*   const table,
               hash_table_key* const key,
               hash_table_key_comparator key_cmp)
{
    if (table         == NULL ||
        table->h_func == NULL ||
        key           == NULL)
        return NULL;

    hash_table_bucket* const bucket = GetBucket (table, key);
    return ListFindNode (bucket, key, key_cmp);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Static functions implementation
//-----------------------------------------------------------------------------

static hash_table_bucket*
GetBucket (hash_table_t* const table,
           hash_table_key* const key)
{
    assert (table);
    assert (table->h_func);
    assert (key);

    const hash_table_index index = table->h_func (key, table->buckets_num);
    if (index >= table->buckets_num) return NULL;

    if (table->buckets[index] == NULL)
        table->buckets[index] = ListConstructor ();

    return table->buckets[index];
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
