/**
 * @file hash_table.h
 * @author SeveraTheDuck
 * @brief Hash table with chain method for solving collisions
 */



#pragma once



#include "doubly_linked_list.h"



//-----------------------------------------------------------------------------
// Hash table structure
//-----------------------------------------------------------------------------

/**
 * @brief Hash table structure
 *
 * @details This structure contains array of buckets and a hash function
 * Other fields are implementation specific
 */
typedef struct hash_table hash_table_t;


/**
 * @brief Hash table node
 *
 * @details This structure contains key and value fields
 */
typedef list_node hash_table_node;


/**
 * @brief Structure to contain a key for a hash function
 */
typedef list_key hash_table_key;


/**
 * @brief Structure to contain a value for a hash function
 */
typedef list_value hash_table_value;


/**
 * @brief Type for hash function return value
 */
typedef size_t hash_table_index;


/**
 * @brief A signature for hash function
 */
typedef
hash_table_index (*hash_function) (hash_table_key* const, const size_t);


/**
 * @brief A enumeration for for a key comparator function result
 */
typedef
enum hash_table_key_cmp_status
{
    HASH_TABLE_KEY_CMP_LESS    = -1,    ///< The first key is less than the second
    HASH_TABLE_KEY_CMP_EQUAL   =  0,    ///< The keys are equal
    HASH_TABLE_KEY_CMP_GREATER =  1     ///< The first key is greater than the second
}
hash_table_key_cmp_status;


/**
 * @brief A type for a key comparator function result
 */
typedef int hash_table_key_cmp_t;


/**
 * @brief A signature for key comparator function
 */
typedef
hash_table_key_cmp_t (*hash_table_key_comparator) (hash_table_key* const,
                                                   hash_table_key* const);


/**
 * @brief Possible error status codes
 */
typedef
enum hash_table_error_status
{
    HASH_TABLE_SUCCESS = 0, ///< function ended successfully
    HASH_TABLE_ERROR   = 1  ///< error occured
}
hash_table_error_status;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Hash table interface
//-----------------------------------------------------------------------------

/**
 * @brief Constructor for hash table structure
 *
 * @param buckets_number Number of buckets in the hash table
 * @param h_func Hash function
 *
 * @retval Pointer to hash_table structure
 * @retval NULL if allocation error occurred
 * @retval NULL if hash function is NULL
 */
hash_table_t*
HashTableConstructor (const size_t buckets_number,
                      hash_function h_func);


/**
 * @brief Destructor for hash table structure
 *
 * @param table Pointer to hash table
 *
 * @return NULL
 *
 * @details Calls destructor for all buckets and nodes
 */
hash_table_t*
HashTableDestructor (hash_table_t* const table);


/**
 * @brief Constructor for hash_table_key structure
 *
 * @param key_buffer A buffer at least key_size bytes long
 * @param key_size Number of bytes to copy
 *
 * @retval Pointer to hash_table_key structure
 * @retval NULL if allocation error occured
 */
hash_table_key*
HashTableKeyConstructor (const void* const key_buffer,
                         const size_t key_size);


/**
 * @brief Destructor for hash_table_key structure
 *
 * @param key A key to be destructed
 *
 * @retval NULL
 */
hash_table_key*
HashTableKeyDestructor (hash_table_key* const key);


/**
 * @brief Constructor for hash_table_value structure
 *
 * @param value_buffer A buffer at least value_size bytes long
 * @param value_size Number of bytes to copy
 *
 * @retval Pointer to hash_table_value structure
 * @retval NULL if allocation error occured
 */
hash_table_value*
HashTableValueConstructor (const void* const value_buffer,
                           const size_t value_size);


/**
 * @brief Destructor for hash_table_value structure
 *
 * @param value A value to be destructed
 *
 * @retval NULL
 */
hash_table_value*
HashTableValueDestructor (hash_table_value* const value);


/**
 * @brief Makes node with given key and value and inserts it into hash table
 *
 * @param table Hash table to insert into
 * @param key Key of the inserting node
 * @param value Value of the inserting node
 *
 * @retval HASH_TABLE_SUCCESS if function ended successfully
 * @retval HASH_TABLE_ERROR if allocation error occured
 * @retval HASH_TABLE_ERROR if bad input received
 */
hash_table_error_status
HashTableInsert (hash_table_t*     const table,
                 hash_table_key*   const key,
                 hash_table_value* const value);


/**
 * @brief Deletes the first node to find with a key equal to the given one
 *
 * @param table Hash table
 * @param key A key to find
 * @param key_cmp A comparator function
 *
 * @retval HASH_TABLE_SUCCESS if function ended successfully
 * @retval HASH_TABLE_ERROR if bad input received
 * @retval HASH_TABLE_ERROR if key not found
 *
 * @details Finds node such that
 * @code key_cmp (key, node->key) == HASH_TABLE_KEY_CMP_EQUAL @endcode
 * and deletes it
 */
hash_table_error_status
HashTableDelete (hash_table_t*   const table,
                 hash_table_key* const key,
                 hash_table_key_comparator key_cmp);


/**
 * @brief Finds node with a key equal to the given one
 *
 * @param table Hash table
 * @param key A key to find
 * @param key_cmp Key comparator function
 *
 * @retval Pointer to the found node
 * @retval NULL if node not found
 * @retval NULL if bad input recieved
 *
 * @details Finds node such that
 * @code key_cmp (key, node->key) == HASH_TABLE_KEY_CMP_EQUAL @endcode
 */
hash_table_node*
HashTableFind (hash_table_t*   const table,
               hash_table_key* const key,
               hash_table_key_comparator key_cmp);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
