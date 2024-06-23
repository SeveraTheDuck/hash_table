/**
 * @file doubly_linked_list.h
 * @author SeveraTheDucl
 * @brief Header for doubly linked list
 */



#pragma once



#include "list_interface.h"
#include <stdlib.h>
#include <string.h>



/**
 * The implementation of doubly linked cycled list using regular nodes
 * @note list_value is allowed to be NULL
 */



//-----------------------------------------------------------------------------
// List structure
//-----------------------------------------------------------------------------

struct list
{
    list_node* head;    ///< first node in the list
    size_t elem_number; ///< number of elements in the list
};


struct list_node
{
    list_key*   key;    ///< key of the node
    list_value* value;  ///< value of the node
    list_node*  next;   ///< next node in the list
    list_node*  prev;   ///< previous node in the list
};


struct list_key
{
    void* key;          ///< key of the node
    size_t key_size;    ///< size of the key in bytes
};


struct list_value
{
    void* value;        ///< value of the node
    size_t value_size;  ///< size of the value in bytes
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
