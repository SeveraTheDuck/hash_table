#include "separation_lib.h"



//-----------------------------------------------------------------------------
// Static functions prototypes
//-----------------------------------------------------------------------------

/**
 * @brief Reads file content into buffer, saves its length
 *
 * @param filename The name of the file to be read
 *
 * @retval Pointer to string_info structure with buffer and length
 * @retval NULL if allocation error occurred
 * @retval NULL if file not found
 * @retval NULL if file is empty
 */
static string_info*
ReadFile (const char* const filename);


/**
 * @brief Get file size in bytes
 *
 * @param file pointer to FILE structure
 *
 * @retval File size in bytes
 */
static size_t
GetFileSize (FILE* const file);


/**
 * @brief Get number of strings separated by separator
 *
 * @param text Pointer to string_info structure to separate
 * @param separator Separator function
 *
 * @retval Number of strings
 */
static size_t
GetStringsNumber (string_info* const text,
                  sep_function separator);


/**
 * @brief Make array of strings separated by separator
 *
 * @param text Pointer to string to separate
 * @param separator Separator function
 * @param strings_num Number of strings
 *
 * @retval Array of strings
 * @retval NULL if allocation error occured
 */
static string_info**
MakeSeparation (string_info* const text,
                sep_function separator,
                const size_t strings_num);

/**
 * @brief Constructor for text_separation structure
 *
 * @param text Pointer to string with whole text
 * @param strings_array Array of strings from the text
 * @param strings_num Number of strings, separated by separator
 *
 * @retval Pointer to text_separation structure
 * @retval NULL if allocation error occured
 */
static text_separation*
TextSeparationConstructor (string_info* const text,
                           string_info** const strings_array,
                           const size_t strings_num);


/**
 * @brief Aborts separation if error occured
 *
 * @param text Pointer to string with whole text
 * @param strings_array Array of strings from the text
 * @param strings_num Number of strings
 *
 * @retval NULL
 */
static text_separation*
AbortSeparation (string_info* const text,
                 string_info** const strings_array,
                 const size_t strings_num);


/**
 * @brief Destructor for whole text from file
 *
 * @param text Pointer to the string with text
 *
 * @retval NULL
 */
static string_info*
BufferDestructor (string_info* const text);


/**
 * @brief Destructor for strings array
 *
 * @param strings_array Pointer to the strings array
 * @param strings_num Number of strings
 *
 * @retval NULL
 *
 * @details Calls for destructor for every string and
 * frees memory allocated for the array
 */
static string_info**
StringsArrayDestructor (string_info** const strings_array,
                        const size_t strings_num);


/**
 * @brief Constructor for string_info structure
 *
 * @param begin_ptr Pointer to begining of the string
 * @param chars_number Number of characters in the string
 *
 * @retval Pointer to the string_info structure
 * @retval NULL if allocation error occured
 */
static string_info*
StringInfoConstructor (char* const begin_ptr,
                       const size_t chars_number);


/**
 * @brief Destructor for the string_info structure
 *
 * @param string Pointer to the string
 *
 * @retval NULL
 */
static string_info*
StringInfoDestructor (string_info* const string);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Interface functions implementation
//-----------------------------------------------------------------------------

text_separation*
SeparateTextFile (const char* const filename,
                  sep_function separator)
{
    if (separator == NULL) return NULL;

    text_separation* text_sep      = NULL;
    string_info*     buffer        = NULL;
    string_info**    strings_array = NULL;
    size_t           strings_num   = 0;

    buffer = ReadFile (filename);
    if (buffer == NULL)
        return AbortSeparation (buffer, strings_array, strings_num);

    strings_num = GetStringsNumber (buffer, separator);

    strings_array = MakeSeparation (buffer, separator, strings_num);
    if (strings_array == NULL)
        return AbortSeparation (buffer, strings_array, strings_num);

    text_sep = TextSeparationConstructor (buffer, strings_array, strings_num);
    if (text_sep == NULL)
        return AbortSeparation (buffer, strings_array, strings_num);

    return text_sep;
}


text_separation*
DestroySeparation (text_separation* const text_sep)
{
    if (text_sep == NULL) return NULL;

    text_sep->text           = BufferDestructor       (text_sep->text);
    text_sep->strings_array  = StringsArrayDestructor (text_sep->strings_array,
                                                       text_sep->strings_number);
    text_sep->strings_number = 0;

    free (text_sep);
    return NULL;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Static functions implementation
//-----------------------------------------------------------------------------

static string_info*
ReadFile (const char* const filename)
{
    if (filename == NULL) return NULL;

    FILE* file = fopen (filename, "rb");
    if (file == NULL) return NULL;

    const size_t file_size = GetFileSize (file);
    if (file_size == 0)
    {
        fclose (file);
        return NULL;
    }

    char* buffer = malloc (file_size);
    if (buffer == NULL)
    {
        fclose (file);
        return NULL;
    }

    fread (buffer, sizeof (char), file_size, file);
    fclose (file);

    string_info* text = StringInfoConstructor (buffer, file_size);
    if (text == NULL) free (buffer);

    return text;
}


static size_t
GetFileSize (FILE* const file)
{
    assert (file);

    fseek (file, 0, SEEK_END);
    const size_t file_size = ftell (file);
    fseek (file, 0, SEEK_SET);

    return file_size;
}


static size_t
GetStringsNumber (string_info* const text,
                  sep_function separator)
{
    assert (text);
    assert (separator);

    const size_t char_num = text->chars_number;
    char* const buffer = text->begin_ptr;

    separator_function_status_t status      = END_SEPARATION;
    separator_function_status_t prev_status = END_SEPARATION;

    size_t strings_num = 0;

    for (size_t i = 0; i < char_num; ++i)
    {
        prev_status = status;
        status = separator (buffer + i);

        if (status == END_SEPARATION) break;

        if (prev_status != NOT_SEPARATOR_ELEMENT &&
            status      == NOT_SEPARATOR_ELEMENT)
            ++strings_num;
    }

    return strings_num;
}


static string_info**
MakeSeparation (string_info* const text,
                sep_function separator,
                const size_t strings_num)
{
    assert (text);
    assert (separator);

    string_info** strings_array = calloc (strings_num, sizeof (string_info*));
    if (strings_array == NULL) return NULL;

    const size_t char_num = text->chars_number;
    char* const buffer = text->begin_ptr;

    char*  cur_string_begin = buffer;
    size_t cur_string_index = 0;
    size_t cur_string_size  = 0;
    separator_function_status_t status = END_SEPARATION;

    for (size_t i = 0; i < char_num; ++i)
    {
        status = separator (buffer + i);

        switch (status)
        {
            case NOT_SEPARATOR_ELEMENT:
                ++cur_string_size;
                break;

            case SEPARATOR_ELEMENT_TAKE:
                ++cur_string_size;
                /* [[fallthrough]]; */

            case SEPARATOR_ELEMENT_NOT_TAKE:
                if (cur_string_size == 0) break;

                strings_array[cur_string_index] =
                    StringInfoConstructor (cur_string_begin, cur_string_size);
                ++cur_string_index;
                cur_string_begin = buffer + i + 1;
                cur_string_size  = 0;
                break;

            case END_SEPARATION:
                return strings_array;
        }
    }

    return strings_array;
}


static text_separation*
TextSeparationConstructor (string_info* const text,
                           string_info** const strings_array,
                           const size_t strings_num)
{
    assert (text);
    assert (strings_array);

    text_separation* text_sep = malloc (sizeof (text_separation));
    if (text_sep == NULL) return NULL;

    text_sep->text           = text;
    text_sep->strings_array  = strings_array;
    text_sep->strings_number = strings_num;

    return text_sep;
}


static text_separation*
AbortSeparation (string_info* const text,
                 string_info** const strings_array,
                 const size_t strings_num)
{
    BufferDestructor (text);
    StringsArrayDestructor (strings_array, strings_num);

    return NULL;
}


static string_info*
BufferDestructor (string_info* const text)
{
    if (text == NULL) return NULL;

    free (text->begin_ptr);
    return StringInfoDestructor (text);
}


static string_info**
StringsArrayDestructor (string_info** const strings_array,
                        const size_t strings_num)
{
    if (strings_array == NULL) return NULL;

    for (size_t i = 0; i < strings_num; ++i)
        strings_array[i] = StringInfoDestructor (strings_array[i]);

    free (strings_array);
    return NULL;
}


static string_info*
StringInfoConstructor (char* const begin_ptr,
                       const size_t chars_number)
{
    string_info* const string = malloc (sizeof (string_info));
    if (string == NULL) return NULL;

    string->begin_ptr    = begin_ptr;
    string->chars_number = chars_number;

    return string;
}


static string_info*
StringInfoDestructor (string_info* const string)
{
    if (string == NULL) return NULL;

    string->begin_ptr    = NULL;
    string->chars_number = 0;
    free (string);

    return NULL;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
