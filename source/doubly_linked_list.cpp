#include "doubly_linked_list.h"



//-----------------------------------------------------------------------------
// Const values
//-----------------------------------------------------------------------------
const char LIST_POISON_VALUE = 0;    ///< Poison char for destructors
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// List functions' implementation
//-----------------------------------------------------------------------------
 list*
ListConstructor (size_t elem_size)
{
    if (elem_size == 0) return nullptr;

    list* const new_list = (list*) malloc (sizeof (list));
    if (new_list == nullptr) return nullptr;

    new_list->head      = nullptr;
    new_list->tail      = nullptr;
    new_list->size      = 0;
    new_list->elem_size = elem_size;

    return new_list;
}

list*
ListDestructor (list* const list)
{
    if (list == nullptr) return nullptr;

    list_node* cur  = list->head;
    list_node* next = nullptr;

    while (cur != nullptr)
    {
        next = cur->next;
        ListNodeDestructor (cur);
        cur = next;
    }

    memset (list, LIST_POISON_VALUE, sizeof (list));
    free (list);

    return nullptr;
}

list_node*
ListInsertElement (list*       const list,
                   list_node*  const prev,
                   const void* const data)
{
    if (list == nullptr || data == nullptr) return nullptr;

    list_node* new_node = nullptr;

    if (prev == nullptr)
        new_node = ListNodeConstructor (nullptr, list->head, data, list->elem_size);
    else
        new_node = ListNodeConstructor (prev,    prev->next, data, list->elem_size);

    if (new_node == nullptr) return nullptr;

    if (new_node->prev == nullptr) list->head = new_node;
    if (new_node->next == nullptr) list->tail = new_node;

    list->size++;

    return new_node;
}

list_error_t
ListDeleteElement (list*      const list,
                   list_node* const node)
{
    if (list == nullptr || node == nullptr) return LIST_ERROR;

    if (list->head == node) list->head = node->next;
    if (list->tail == node) list->tail = node->prev;

    if (ListNodeDelete (node) == LIST_NODE_ERROR) return LIST_ERROR;

    list->size--;

    return LIST_SUCCESS;
}

list_node*
ListFindElement (list*       const list,
                 const void* const data,
                 int (*cmp) (const void*, const void*))
{
    if (list == nullptr || data == nullptr) return nullptr;

    list_node* cur = list->head;

    while (cur != nullptr)
    {
        if (cmp == nullptr && memcmp (cur->data, data, list->elem_size) == 0)
            return cur;

        else if (cmp (cur->data, data) == 0)
            return cur;

        cur = cur->next;
    }

    return nullptr;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// List node functions' implementation
//-----------------------------------------------------------------------------
list_node*
ListNodeConstructor (list_node*  const prev,
                     list_node*  const next,
                     const void* const data,
                     const size_t elem_size)
{
    list_node* const node = (list_node*) malloc (sizeof (list_node));
    if (node == nullptr) return nullptr;

    node->data = calloc (1, elem_size);
    if (node->data == nullptr)
        return ListNodeDestructor (node);

    if (data)
        memcpy (node->data, data, elem_size);

    if (prev)
    {
        prev->next = node;
        node->prev = prev;
    }

    if (next)
    {
        node->next = next;
        next->prev = node;
    }

    node->elem_size = elem_size;

    return node;
}

list_node*
ListNodeDestructor (list_node* const node)
{
    if (node == nullptr) return nullptr;

    if (node->data != nullptr)
        memset (node->data, LIST_POISON_VALUE, node->elem_size);

    free (node->data);

    memset (node, LIST_POISON_VALUE, sizeof (list_node));
    free (node);

    return nullptr;
}

list_node_error_t
ListNodeDelete (list_node* const node)
{
    if (node == nullptr) return LIST_NODE_ERROR;

    list_node* prev = node->prev;
    list_node* next = node->next;

    if (prev != nullptr) prev->next = next;
    if (next != nullptr) next->prev = prev;

    ListNodeDestructor (node);

    return LIST_NODE_SUCCESS;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
