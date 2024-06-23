#include "separator_function.h"



//-----------------------------------------------------------------------------
// Separator function implementation
//-----------------------------------------------------------------------------

separator_function_status_t
Separator (const char* const symbol)
{
    assert (symbol);

    char c = *symbol;
    if (isalpha (c)) return NOT_SEPARATOR_ELEMENT;
    return SEPARATOR_ELEMENT_NOT_TAKE;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
