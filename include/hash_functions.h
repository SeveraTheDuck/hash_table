/**
 * @file hash_functions.h
 *
 * @author SeveraTheDuck
 *
 * @brief Hash functions with one signature
 */

/**
 * In this file I experiment with doxygen groups to know
 * what the heck do they do
 */



#pragma once



#include "hash_table.h"
#include "doubly_linked_list.h"
#include <assert.h>



//-----------------------------------------------------------------------------
// Hash functions prototypes
//-----------------------------------------------------------------------------
/**
 * @defgroup hash_functions
 *
 * @brief Hash functions with one signature:
 * @code
 * hash_table_index (*hash_function) (hash_table_key* const, const size_t);
 * @endcode
 *
 * @details
 * Each function gets key and number of buckets, so I don't write params docs
 * for each function - they are all the same
 * In this project we work with character strings, so hash functions
 * are mostly for strings
 *
 * @{
 */

/**
 * @brief Returns zero, that's it
 */
hash_table_index
HashFunctionZero (hash_table_key* const key,
                  const size_t buckets_num);


/**
 * @brief Returns ASCII code of the first symbol
 */
hash_table_index
HashFunctionFirstASCII (hash_table_key* const key,
                        const size_t buckets_num);


/**
 * @brief Returns length of the received string
 */
hash_table_index
HashFunctionStringLength (hash_table_key* const key,
                          const size_t buckets_num);


/**
 * @brief Returns sum of ASCII codes of all symbols in the string
 */
hash_table_index
HashFunctionSumASCII (hash_table_key* const key,
                      const size_t buckets_num);


/**
 * @brief Returns some bit magic with left shift
 *
 * @see Look at the implementation for details
 */
hash_table_index
HashFunctionRol (hash_table_key* const key,
                 const size_t buckets_num);


/**
 * @brief Returns some bit magic with right shift
 *
 * @see Look at the implementation for details
 */
hash_table_index
HashFunctionRor (hash_table_key* const key,
                 const size_t buckets_num);

/**
 * @brief Famous string hash function
 *
 * @see <a href="http://www.cse.yorku.ca/~oz/hash.html">Djb2 implementation</a>
 */
hash_table_index
HashFunctionDjb2 (hash_table_key* const key,
                  const size_t buckets_num);


/**
 * @brief Famous string hash function
 *
 * @see <a href="https://en.wikipedia.org/wiki/Cyclic_redundancy_check">Crc32</a>
 * @see <a href="http://en.wikipedia.org/wiki/Computation_of_cyclic_redundancy_checks">
 * Crc32 implementation</a>
 */
hash_table_index
HashFunctionCrc32 (hash_table_key* const key,
                   const size_t buckets_num);

/** @} */ // end of hash_functions group
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
