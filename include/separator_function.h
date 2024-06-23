/**
 * @file separator_function.h
 * @author SeveraTheDuck
 * @brief Separator function for text to separate it into words
 */



#pragma once



#include "separation_lib.h"
#include "ctype.h"



//-----------------------------------------------------------------------------
// Separator function prototype
//-----------------------------------------------------------------------------

/**
 * @brief This function decides whether the symbol is a separator or not
 *
 * @param symbol Pointer to the symbol
 *
 * @return @see separator_function_status
 */
separator_function_status_t
Separator (const char* const symbol);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
