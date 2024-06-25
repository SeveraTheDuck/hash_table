#include "common.h"



//-----------------------------------------------------------------------------
// Consts
//-----------------------------------------------------------------------------

/// @brief exe_file, text_file_name, buckets_number, hash_function_number
static const int TEST_HASH_FUNCTION_ARGS_NUMBER = 4;


/// @brief text_file_name argument index
static const size_t TEST_HASH_FUNCTION_TEXT_ARG = 1;


/// @brief buckets_number argument index
static const size_t TEST_HASH_FUNCTION_BUCKETS_NUMBER_ARG = 2;


/// @brief hash_function_number argument index
static const size_t TEST_HASH_FUNCTION_INDEX_ARG = 3;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Output static functions prototypes
//-----------------------------------------------------------------------------

/**
 * @brief Prints number of buckets, number of elements in buckets
 * and dispersion for the elements distribution
 *
 * @param table Filled hash table
 */
static void
PrintResults (const hash_table_t* const table);


/**
 * @brief Calculates dispersion for the elements distribution
 *
 * @param table Filled hash table
 *
 * @details $D\ksi = E(\ksi^2) - (E\ksi)^2$,
 * where D is dispersion, E is expected number, $\ksi$ is number of elements
 * in the bucket
 * If the distribution is uniform, k is number of buckets,
 * n is total number of elements, $\ksi_i$ is number of elements in the bucket,
 * $D\ksi = \frac{\[ \sum_{i=1}^{k} \ksi_i \]}{k} - (\frac{n}{k})^2$
 */
static double
CalculateDispersion (const hash_table_t* const table);


/**
 * @brief Prints number of buckets and number of elements in each bucket
 *
 * @param table Hash table
 */
static void
PrintBucketsCountArray (const hash_table_t* const table);


/**
 * @brief Gets number of elements in the bucket
 *
 * @param bucket The bucket
 *
 * @retval Number of elements
 * @retval 0 if bucket is NULL
 */
static size_t
GetBucketElemNumber (const hash_table_bucket* const bucket);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Output static functions implementation
//-----------------------------------------------------------------------------

static void
PrintResults (const hash_table_t* const table)
{
    assert (table);

    PrintBucketsCountArray (table);
    fprintf (stderr, "Dispersion %lf\n", CalculateDispersion (table));
}


static double
CalculateDispersion (const hash_table_t* const table)
{
    assert (table);

    const size_t buckets_number = table->buckets_num;
    size_t cur_bucket_count     = 0;
    size_t sum_of_squares       = 0;

    hash_table_bucket** buckets_array = table->buckets;

    for (size_t i = 0; i < buckets_number; ++i)
    {
        cur_bucket_count = GetBucketElemNumber (buckets_array[i]);
        sum_of_squares  += cur_bucket_count * cur_bucket_count;
    }

    const double exp_value = (double) table->elem_number / buckets_number;
    const double exp_value_squares = (double) sum_of_squares / buckets_number;

    return exp_value_squares - exp_value * exp_value;
}


static void
PrintBucketsCountArray (const hash_table_t* const table)
{
    assert (table);
    assert (table->buckets);

    const size_t buckets_number = table->buckets_num;
    hash_table_bucket** buckets_array = table->buckets;

    for (size_t i = 0; i < buckets_number; ++i)
        printf ("%zu %zu\n", i, GetBucketElemNumber (buckets_array[i]));
}


static size_t
GetBucketElemNumber (const hash_table_bucket* const bucket)
{
    if (bucket == NULL) return 0;
    return bucket->elem_number;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------------

int main (const int argc, const char** const argv)
{
    assert (argc == TEST_HASH_FUNCTION_ARGS_NUMBER);
    assert (argv);

    const size_t buckets_number =
        atoll (argv[TEST_HASH_FUNCTION_BUCKETS_NUMBER_ARG]);

    const size_t hash_function_number =
        atoll (argv[TEST_HASH_FUNCTION_INDEX_ARG]);

    hash_function h_func = GetHashFunctionPointer (hash_function_number);
    assert (h_func);

    hash_table_t* table = FillHashTable (argv[TEST_HASH_FUNCTION_TEXT_ARG],
                                         buckets_number, h_func);
    assert (table);

    PrintResults (table);

    table = HashTableDestructor (table);
    return 0;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
