#pragma once



#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>



//-----------------------------------------------------------------------------
// List struct
//-----------------------------------------------------------------------------
struct list
{
    list_node*  head; ///< Head of the list
    list_node*  tail; ///< Tail of the list
    size_t      size; ///< Number of elements in the list
    size_t elem_size; ///< Size of each element in the list in bytes
};

enum list_error_status
{
    LIST_SUCCESS = 0, ///< Function executed successfully
    LIST_ERROR   = 1  ///< Function failed
};

/// @brief Type to contain the error status of the list functions
typedef int list_error_t;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// List node struct
//-----------------------------------------------------------------------------
struct list_node
{
    list_node*  next; ///< Pointer to the next node 
    list_node*  prev; ///< Pointer to the previous node
    void*       data; ///< Pointer to data
    size_t elem_size; ///< Size of data in bytes
};

enum list_node_error_status
{
    LIST_NODE_SUCCESS = 0,  ///< Function executed successfully
    LIST_NODE_ERROR   = 1   ///< Function failed
};

/// @brief Type to contain the error status of the list node functions
typedef int list_node_error_t;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// List functions' prototypes
//-----------------------------------------------------------------------------
/**
 * @brief Creates a new list with the given element size
 * @param elem_size The size of each element in the list in bytes
 * @return A pointer to the  created list, or NULL if there was an error
 */
list*
ListConstructor (size_t elem_size);

/**
 * @brief Destroys a list and all its elements
 * @param list The list to be destroyed
 * @return NULL
 */
list*
ListDestructor (struct list* const list);

/**
 * @brief Inserts an element into the list
 * @param list The list to insert the element into
 * @param prev The node after which the new node should be inserted
 * @param data The data to be stored in the new node
 * @return A pointer to the new node, or NULL if there was an error
 */
list_node*
ListInsertElement (list* const list,
                   list_node* const prev,
                   const  void* const data);

/**
 * @brief Deletes an element from the list
 * @param list The list from which the element should be deleted
 * @param node The node to be deleted
 * @return LIST_SUCCESS if the element was successfully deleted, or LIST_ERROR if there was an error
 */
list_error_t
ListDeleteElement (list* const list,
                   list_node* const node);

/**
 * @brief Searches for an element in the list using a comparison function
 * @param list The list to search in
 * @param data The data to search for
 * @param cmp The comparison function to use for comparing elements. If NULL, memcmp is used
 * @return A pointer to the node containing the found element, or NULL if the element was not found
 */
list_node*
ListFindElement (list* const list,
                 const  void* const data,
                 int (*cmp) (const void*, const void*));
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// List node functions' prototypes
//-----------------------------------------------------------------------------
/**
 * @brief Creates a new list node with the given prev, next, data, and elem_size
 * @param prev The previous node
 * @param next The next node
 * @param data The data to be stored in the node
 * @param elem_size The size of the data in bytes
 * @return A pointer to the new node, or NULL if there was an error
 */
list_node*
ListNodeConstructor (list_node* const prev,
                     list_node* const next,
                     const  void* const data,
                     const  size_t elem_size);

/**
 * @brief Destroys a list node and frees its memory
 * @param node The node to be destroyed
 * @return NULL
 */
list_node*
ListNodeDestructor (list_node* const node);

/**
 * @brief Deletes a node from a doubly linked list and connects prev and next nodes. Calls for ListNodeDestructor()
 * @param node The node to be deleted
 * @return LIST_SUCCESS if the node was successfully deleted, or LIST_ERROR if there was an error
 */
list_error_t
ListNodeDelete (list_node* const node);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
