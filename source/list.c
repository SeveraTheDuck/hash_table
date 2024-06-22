#include "list.h"
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



//-----------------------------------------------------------------------------
// Static functions prototypes
//-----------------------------------------------------------------------------

static list_key*
ListKeyCopy (const list_key* const key);


static list_value*
ListValueCopy (const list_value* const value);


static list_error_status
ListLinkNodes (list_node* const node1,
               list_node* const node2);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Interface functions implementation
//-----------------------------------------------------------------------------

list_t*
ListConstructor (void)
{
    list_t* const list = calloc (1, sizeof (list_t));
    return list; // if NULL returns NULL
}


list_t*
ListDestructor (list_t* const list)
{
    if (list == NULL) return NULL;

    const size_t elem_number = list->elem_number;
    for (size_t i = 0; i < elem_number; ++i)
        ListDeleteNode (list, list->head);

    free (list);
    return NULL;
}


list_error_status
ListInsert (list_t*     const list,
            list_node*  const prev_node,
            list_key*   const key,
            list_value* const value)
{
    if (list == NULL ||
        key  == NULL)
        return LIST_ERROR;

    list_node* const node = ListNodeConstructor (key, value);
    if (node == NULL) return LIST_ERROR;

    if (list->head == NULL)
        list->head = node;

    ++list->elem_number;

    if (prev_node == NULL)
        return ListLinkNodes (node, list->head);

    return ListLinkNodes (prev_node, node);
}


list_error_status
ListPushBack (list_t*     const list,
              list_key*   const key,
              list_value* const value)
{
    if (list == NULL) return LIST_ERROR;

    if (list->head == NULL)
        return ListInsert (list, NULL, key, value);

    return ListInsert (list, list->head->prev, key, value);
}


list_error_status
ListPushFront (list_t*     const list,
               list_key*   const key,
               list_value* const value)
{
    if (ListInsert (list, NULL, key, value) == LIST_ERROR)
        return LIST_ERROR;

    list->head = list->head->prev;

    return LIST_SUCCESS;
}


list_error_status
ListDeleteNode (list_t*    const list,
                list_node* const node)
{
    if (list == NULL ||
        node == NULL)
        return LIST_ERROR;

    list_node* const head = list->head;

    if (node == head) list->head = head->next;

    ListLinkNodes (node->prev, node->next);

    ListNodeDestructor (node);

    --list->elem_number;

    return LIST_SUCCESS;
}


list_node*
ListFindNode (list_t* const list,
              list_key* const key,
              list_key_cmp key_cmp)
{
    if (list    == NULL ||
        key     == NULL ||
        key_cmp == NULL)
        return NULL;

    const size_t elem_number = list->elem_number;
    list_node* cur_node = list->head;

    for (size_t i = 0; i < elem_number; ++i)
    {
        if (key_cmp (cur_node->key, key) == LIST_KEY_CMP_EQUAL)
            return cur_node;

        cur_node = cur_node->next;
    }

    return NULL;
}


list_node*
ListNodeConstructor (const list_key* const key,
                     const list_value* const value)
{
    list_node* const node = (list_node*) malloc (sizeof (list_node));
    if (node == NULL) return NULL;

    node->key   = ListKeyCopy   (key);
    node->value = ListValueCopy (value);
    node->next  = node;
    node->prev  = node;

    return node;
}


list_node*
ListNodeDestructor (list_node* const node)
{
    if (node == NULL) return NULL;

    node->key   = ListKeyDestructor   (node->key);
    node->value = ListValueDestructor (node->value);
    node->next  = NULL;
    node->prev  = NULL;

    free (node);
    return NULL;
}


list_key*
ListKeyConstructor (const void* const key_buffer,
                    const size_t key_size)
{
    list_key* const key = calloc (1, sizeof (list_key));
    if (key == NULL) return NULL;

    if (key_buffer == NULL || key_size == 0)
        return key;

    key->key_size = key_size;
    key->key = malloc (key_size);
    if (key->key == NULL)
        return ListKeyDestructor (key);

    memcpy (key->key, key_buffer, key_size);

    return key;
}


list_key*
ListKeyDestructor (list_key* const key)
{
    if (key == NULL) return NULL;

    free (key->key);
    free (key);
    return NULL;
}


list_value*
ListValueConstructor (const void* const value_buffer,
                      const size_t value_size)
{
    list_value* const value = calloc (1, sizeof (list_value));
    if (value == NULL) return NULL;

    if (value_buffer == NULL || value_size == 0)
        return value;

    value->value_size = value_size;
    value->value = malloc (value_size);
    if (value->value == NULL)
        return ListValueDestructor (value);

    memcpy (value->value, value_buffer, value_size);

    return value;
}


list_value*
ListValueDestructor (list_value* const value)
{
    if (value == NULL) return NULL;

    free (value->value);
    free (value);
    return NULL;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Static functions implementation
//-----------------------------------------------------------------------------

static list_key*
ListKeyCopy (const list_key* const key)
{
    if (key == NULL) return NULL;

    return ListKeyConstructor (key->key, key->key_size);
}


static list_value*
ListValueCopy (const list_value* const value)
{
    if (value == NULL) return NULL;

    return ListValueConstructor (value->value, value->value_size);
}


static list_error_status
ListLinkNodes (list_node* const node1,
               list_node* const node2)
{
    if (node1 == NULL ||
        node2 == NULL)
        return LIST_SUCCESS;

    list_node* const first_next  = node1->next;
    list_node* const second_prev = node2->prev;

    node1->next = node2;
    node2->prev = node1;

    second_prev->next = first_next;
    first_next->prev = second_prev;

    return LIST_SUCCESS;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
