#include "hash_functions.h"
#include "separation_lib.h"
#include <ctype.h>



//-----------------------------------------------------------------------------
// Common test functions prototypes
//-----------------------------------------------------------------------------

/**
 * @brief Splits file into words and fills hash table with them
 *
 * @param filename Name of the file to be splited
 * @param buckets_number Number of buckets in the hash table
 * @param h_func Hash function
 *
 * @retval Pointer to hash table
 *
 * @note Function falls with assert() if filename or hash function NULL,
 * file not found or empty or allocation error occurred
 */
hash_table_t*
FillHashTable (const char* const filename,
               const size_t buckets_number,
               hash_function h_func);


/**
 * @brief Key comparator function for hash table
 *
 * @param key1 First key
 * @param key2 Second key
 *
 * @retval @see hash_table_key_cmp_status
 */
hash_table_key_cmp_t
KeyCmpFunction (hash_table_key* const key1,
                hash_table_key* const key2);


/**
 * @brief This function decides whether the symbol is a separator or not
 *
 * @param symbol Pointer to the symbol
 *
 * @return @see separator_function_status
 */
separator_function_status_t
Separator (const char* const symbol);


/**
 * @brief Gets pointer to the hash function
 *
 * @param hash_function_index Index of the hash function
 *
 * @retval Pointer to the hash function
 * @retval NULL if index out of range
 */
hash_function
GetHashFunctionPointer (const size_t hash_function_index);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
