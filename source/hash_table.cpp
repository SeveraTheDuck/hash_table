#include "hash_table.h"



const char HASH_POISON_BYTE = 0x0;

//-----------------------------------------------------------------------------
// Hash table functions' implementation
//-----------------------------------------------------------------------------
hash_table*
HashTableConstructor (const size_t capacity,
                      size_t (*hash_function) (const void* const key,
                                               const size_t capacity))
{
    hash_table* const table = (hash_table*) malloc (sizeof (hash_table));
    if (table == NULL) return NULL;

    table->size          = 0;
    table->capacity      = capacity;
    table->hash_function = hash_function;

    table->list_array = (list**) calloc (capacity, sizeof (list*));
    if (table->list_array == NULL)
    {
        free (table);
        return NULL;
    }

    return table;
}

hash_table*
HashTableDestructor (hash_table* const table)
{
    if (table == NULL) return NULL;
    if (table->list_array == NULL)
    {
        memset (table, HASH_POISON_BYTE, sizeof (hash_table));
        free (table);
        return NULL;
    }

    for (size_t i = 0; i < table->capacity; i++)
        table->list_array[i] = ListDestructor (table->list_array[i]);

    free (table->list_array);
    memset (table, HASH_POISON_BYTE, sizeof (hash_table));

    free (table);
    return NULL;
}

list_node*
HashTableAddElement (hash_table* const table,
                     const hash_table_pair* const data)
{
    if (table                == NULL ||
        table->list_array    == NULL ||
        table->hash_function == NULL ||
        data                 == NULL ||
        data->key            == NULL ||
        data->value          == NULL)
        return NULL;

    if (HashTableFindElement (table, data) != NULL)
        return NULL;

    const size_t list_index = table->hash_function (data->key, table->capacity);

    if (list_index >= table->capacity) return NULL;

    if (table->list_array[list_index] == NULL)
    {
        table->list_array[list_index] =
            ListConstructor (sizeof (hash_table_pair));
        if (table->list_array[list_index] == NULL) return NULL;
    }

    hash_table_pair* data_copy =
        HashTablePairConstructor (data->key, data->value, data->key_size, data->value_size);
    if (data_copy == NULL) return NULL;

    list_node* const new_node =
        ListInsertElement (table->list_array[list_index],
                           table->list_array[list_index]->tail,
                           data_copy);

    if (new_node == NULL) return NULL;

    table->size++;

    return new_node;
}

hash_table_error_t
HashTableDeleteElement (hash_table* const table,
                        list_node*  const node)
{
    if (table                == NULL ||
        table->list_array    == NULL ||
        table->hash_function == NULL ||
        node                 == NULL)
        return HASH_TABLE_ERROR;

    const size_t list_index =
        table->hash_function (HashTablePairKey (node->data), table->capacity);
    if (list_index >= table->capacity) return HASH_TABLE_ERROR;

    list* list = table->list_array[list_index];
    if (list == NULL) return HASH_TABLE_ERROR;

    node->data = HashTablePairDestructor (node->data);

    if (ListDeleteElement (list, node) == LIST_ERROR)
        return HASH_TABLE_ERROR;

    table->size--;
    return HASH_TABLE_SUCCESS;
}

list_node*
HashTableFindElement (hash_table* const table,
                      const hash_table_pair* const data)
{
    if (table             == NULL ||
        table->list_array == NULL ||
        data              == NULL ||
        data->key         == NULL ||
        data->value       == NULL)
        return NULL;

    const size_t list_index = table->hash_function (data->key, table->capacity);

    if (list_index >= table->capacity) return NULL;

    if (table->list_array[list_index] == NULL) return NULL;

    list_node* node =
        ListFindElement (table->list_array[list_index], data, NULL);

    return node;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Hash table pair functions' implementation
//-----------------------------------------------------------------------------
hash_table_pair*
HashTablePairConstructor (const void* const key,
                          const void* const value,
                          const size_t key_size,
                          const size_t value_size)
{
    hash_table_pair* const pair =
        (hash_table_pair* const) calloc (1, sizeof (hash_table_pair));
    if (pair == NULL) return NULL;

    pair->key = calloc (1, key_size);
    if (pair->key == NULL)
    {
        free (pair);
        return NULL;
    }

    pair->value = calloc (1, value_size);
    if (pair->value == NULL)
    {
        free (pair->key);
        free (pair);
        return NULL;
    }

    if (key != NULL)
        memcpy (pair->key, key, key_size);

    pair->key_size = key_size;

    if (value != NULL)
        memcpy (pair->value, value, value_size);

    pair->value_size = value_size;

    return pair;
}

hash_table_pair*
HashTablePairDestructor (hash_table_pair* const pair)
{
    if (pair == NULL) return NULL;

    free (pair->key);
    free (pair->value);

    memset (pair, HASH_POISON_BYTE, sizeof (hash_table_pair));

    free (pair);
    return NULL;
}

int
HashTablePairCompare (const hash_table_pair* const pair1,
                      const hash_table_pair* const pair2)
{
    if (pair1        == NULL || pair2        == NULL ||
        pair1->key   == NULL || pair2->key   == NULL ||
        pair1->value == NULL || pair2->value == NULL)
        return HASH_TABLE_PAIRS_NOT_EQUAL;

    if (memcmp (pair1->key,   pair2->key,   pair1->key_size)   == 0 &&
        memcmp (pair1->value, pair2->value, pair1->value_size) == 0)
        return HASH_TABLE_PAIRS_EQUAL;

    return HASH_TABLE_PAIRS_NOT_EQUAL;
}

void*
HashTablePairKey (hash_table_pair* const pair)
{
    if (pair == NULL) return NULL;
    return pair->key;
}

void*
HashTablePairValue (hash_table_pair* const pair)
{
    if (pair == NULL) return NULL;
    return pair->value;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------