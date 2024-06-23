#include "hash_functions.h"
#include "separator_function.h"



const size_t HT_SIZE = 0x400;



void
PrintString (const string_info* const string)
{
    assert (string);

    const size_t string_length = string->chars_number;
    const char* const str = string->begin_ptr;

    for (size_t i = 0; i < string_length; ++i)
        putchar (str[i]);

    putchar ('\n');
}


void
PrintSeparation (const text_separation* const text_sep)
{
    assert (text_sep);

    const size_t strings_number = text_sep->strings_number;

    for (size_t i = 0; i < strings_number; ++i)
        PrintString (text_sep->strings_array[i]);
}



int main (void)
{
    text_separation* text_sep = SeparateTextFile ("text.txt", Separator);
    assert (text_sep);

    PrintSeparation (text_sep);

    text_sep = DestroySeparation (text_sep);
    return 0;
}
