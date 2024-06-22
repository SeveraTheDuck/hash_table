/**
 * @note
 * This file contains list interface only, no particular implementation details
 * It is intended to be a way to use one interface for multiple implementations
 *
 * For example, you may implement it using a dynamic array, make it singly or
 * doubly linked etc.
 */



#pragma once



#include <stdlib.h>



//-----------------------------------------------------------------------------
// List structure
//-----------------------------------------------------------------------------

/**
 * @brief List structure
 *
 * @details
 * This structure should contain a pointer to the first element of the list
 * Other fields are defined by the implementation
 */
typedef struct list list_t;


/**
 * @brief List node structure
 *
 * @details
 * This structure should contain list_key and list_value
 * Other fields are defined by the implementation
 */
typedef struct list_node list_node;


/**
 * @brief Structure to contatin a key of the list node
 */
typedef struct list_key list_key;


/**
 * @brief Structure to contatin a value of the list node
 */
typedef struct list_value list_value;


/**
 * @brief A enumeration for a key comparison function result
 */
typedef
enum list_key_cmp_status
{
    LIST_KEY_LESS    = -1,  ///< The first key is less than the second
    LIST_KEY_EQUAL   =  0,  ///< The keys are equal
    LIST_KEY_GREATER =  1   ///< The first key is greater than the second
}
list_key_cmp_status;


/**
 * @brief A signature for key comparator function
 */
typedef list_key_cmp_status (*list_key_cmp)(list_key* const, list_key* const);


/**
 * @brief Possible error status codes
 */
typedef
enum list_error_status
{
    LIST_SUCCESS = 0,   ///< Function ended successfully
    LIST_ERROR   = 1    ///< Error occured
}
list_error_status;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Interface
//-----------------------------------------------------------------------------

/**
 * @brief Constructor for a list structure
 *
 * @retval Pointer to the list structure
 * @retval NULL if allocation error occurred
 */
list_t*
ListConstructor (void);


/**
 * @brief Destructor for a list structure
 *
 * @param list A pointer to the list structure
 *
 * @retval NULL
 *
 * @note Calls destructor for all nodes in the list
 */
list_t*
ListDestructor (list_t* const list);


/**
 * @brief Insert node with given key and value after a given prev_node
 *
 * @param list A pointer to the list where to insert
 * @param prev_node A pointer to the node to insert after
 * @param key A key of the node to insert
 * @param value A value of the node to insert
 *
 * @retval LIST_SUCCESS if function ended successfully
 * @retval LIST_ERROR   if error occured (implementation defined)
 *
 * @note Use ListPushFront() function to push into head of the list
 */
list_error_status
ListInsert (list_t*     const list,
            list_node*  const prev_node,
            list_key*   const key,
            list_value* const value);


/**
 * @brief Insert in the end of the list
 *
 * @param list A pointer to the list where to insert
 * @param key A key of the node to insert
 * @param value A value of the node to insert
 *
 * @retval LIST_SUCCESS if function ended successfully
 * @retval LIST_ERROR   if error occured (implementation defined)
 */
list_error_status
ListPushBack (list_t*     const list,
              list_key*   const key,
              list_value* const value);


/**
 * @brief Insert in the beginning of the list
 *
 * @param list A pointer to the list where to insert
 * @param key A key of the node to insert
 * @param value A value of the node to insert
 *
 * @retval LIST_SUCCESS if function ended successfully
 * @retval LIST_ERROR   if error occured (implementation defined)
 */
list_error_status
ListPushFront (list_t*     const list,
               list_key*   const key,
               list_value* const value);


/**
 * @brief Delete node from the list
 *
 * @param list A pointer to the list to delete from
 * @param node A pointer to the node to be deleted
 *
 * @retval LIST_SUCCESS if function ended successfully
 * @retval LIST_ERROR   if error occured (implementation defined)
 */
list_error_status
ListDeleteNode (list_t*    const list,
                list_node* const node);


/**
 * @brief A function which searches for a node with the given key
 *
 * @param list A pointer to the list to find the node
 * @param key  A pointer to the key to search
 * @param key_cmp A pointer to the comparator function
 *
 * @retval Pointer to the found node in the list
 * @retval NULL if node not found or invalid arguments recieved
 */
list_node*
ListFindNode (list_t* const list,
              list_key* const key,
              list_key_cmp key_cmp);


/**
 * @brief Constructor for list key structure
 *
 * @param key_buffer A buffer at least key_size bytes long
 * @param key_size Number of bytes to copy
 *
 * @retval Pointer to list_key structure
 * @retval NULL if error occured
 */
list_key*
ListKeyConstructor (const void* const key_buffer,
                    const size_t key_size);


/**
 * @brief Destructor for list key structure
 *
 * @param key A pointer to the key to be destructed
 *
 * @retval NULL
 */
list_key*
ListKeyDestructor (list_key* const key);


/**
 * @brief Constructor for list value structure
 *
 * @param value_buffer A buffer at least value_size bytes long
 * @param value_size Number of bytes to copy
 *
 * @retval Pointer to the list_value structure
 * @retval NULL if error occured
 */
list_value*
ListValueConstructor (const void* const value_buffer,
                      const size_t value_size);


/**
 * @brief Destructor for list value structure
 *
 * @param value A pointer to the value to be destructed
 *
 * @retval NULL
 */
list_value*
ListValueDestructor (list_value* const value);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
