#pragma once



#include "doubly_linked_list.h"



//-----------------------------------------------------------------------------
// Hash table struct and erorrs.
//-----------------------------------------------------------------------------
struct hash_table
{
    list** list_array;   ///< array of lists
    size_t size;         ///< number of elements in the hash table
    size_t capacity;     ///< number of keys in the hash table

    size_t (*hash_function) (const void* const key,
                             const size_t capacity);
};

struct hash_table_pair
{
    void* key;
    void* value;

    size_t key_size;    ///< size of key in bytes
    size_t value_size;  ///< size of value in bytes
};

enum hash_table_pair_cmp_status
{
    HASH_TABLE_PAIRS_EQUAL     = 0,
    HASH_TABLE_PAIRS_NOT_EQUAL = 1
};

enum hash_table_error_status
{
    HASH_TABLE_SUCCESS = 0,
    HASH_TABLE_ERROR   = 1
};

typedef int hash_table_error_t;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Hash table functions' prototypes
//-----------------------------------------------------------------------------
struct hash_table*
HashTableConstructor (const size_t capacity,
                      size_t (*hash_function) (const void* const key,
                                               const size_t capacity));

struct hash_table*
HashTableDestructor (struct hash_table* const table);

struct list_node*
HashTableAddElement (struct hash_table* const table,
                     const struct hash_table_pair* const data);

hash_table_error_t
HashTableDeleteElement (struct hash_table* const table,
                        struct list_node* const data);

struct list_node*
HashTableFindElement (struct hash_table* const table,
                      const struct hash_table_pair* const data);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Hash table pair functions' prototypes
//-----------------------------------------------------------------------------
struct hash_table_pair*
HashTablePairConstructor (const void* const key,
                          const void* const value,
                          const size_t key_size,
                          const size_t value_size);

struct hash_table_pair*
HashTablePairDestructor (struct hash_table_pair* const pair);

int
HashTablePairCompare (const struct hash_table_pair* const pair1,
                      const struct hash_table_pair* const pair2);

void*
HashTablePairKey (struct hash_table_pair* const pair);

void*
HashTablePairValue (struct hash_table_pair* const pair);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------