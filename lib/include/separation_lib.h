/**
 * @file separation_lib.h
 *
 * @author SeveraTheDuck
 *
 * @brief The library to separate file into strings depending on taken
 * function-separator
 */



#pragma once



#include <assert.h>
#include <stdio.h>
#include <stdlib.h>



//-----------------------------------------------------------------------------
// Text separation structures
//-----------------------------------------------------------------------------

/**
 * @brief String structure
 *
 * @note The string might be not null terminated
 */
typedef
struct string_info
{
    char* begin_ptr;        ///< begin of string
    size_t chars_number;    ///< number of characters
}
string_info;


/**
 * @brief Structure which contains text and its separation
 *
 * @details String_info structures contain pointers to the text
 */
typedef
struct text_separation
{
    string_info* text;              ///< buffer with whole text
    string_info** strings_array;    ///< array of strings
    size_t strings_number;          ///< number of strings
}
text_separation;


/**
 * @brief Enumeration of possible error statuses
 */
enum separation_errors
{
    SEPARATION_SUCCESS = 0, ///< no error occured, success
    SEPARATION_ERROR   = 1  ///< error occured (allocation, nullptr etc)
};


/**
 * @brief Type to contatin error status
 */
typedef size_t separation_error_t;


/**
 * @brief Enumeration for separator function return values
 */
enum separator_function_status
{
    END_SEPARATION             = 0, ///< end separation, do not take char into string
    SEPARATOR_ELEMENT_TAKE     = 1, ///< end string, take element into string
    SEPARATOR_ELEMENT_NOT_TAKE = 2, ///< end string, do not take element into string
    NOT_SEPARATOR_ELEMENT      = 3  ///< take element into string, continue string
};


/**
 * @brief Separator function return type
 */
typedef size_t separator_function_status_t;


/**
 * @brief Separator function signature
 */
typedef
separator_function_status_t (*sep_function) (const char* const);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Text separation interface
//-----------------------------------------------------------------------------

/**
 * @brief Opens file and separates its text into strings, closes file
 *
 * @param filename Name of file to separate
 * @param separator Separator function
 *
 * @retval Pointer to the text_separation structure
 * @retval NULL if allocation error occured
 * @retval NULL if file open  error occured
 * @retval NULL if separator function is NULL
 * @retval NULL if file is empty
 *
 * @details This function opens file with name filename and makes an array
 * of strings
 *
 * @note The strings are not guaranteed to be null terminated
 */
text_separation*
SeparateTextFile (const char* const filename,
                  sep_function separator);

/**
 * @brief Destructor for text_separation structure
 *
 * @param text_sep Pointer to the text_separation structure
 *
 * @retval NULL
 *
 * @details Frees memory of text buffer, strings_array and text_separation
 * structure itself
 */
text_separation*
DestroySeparation (text_separation* const text_sep);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
