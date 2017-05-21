/*
 * Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
 *
 * Licensed under the MIT License. See LICENSE at top level directory.
 */

#include "list.h"

struct list_node
{
    struct list_node *next;
    const void *data;
};

struct list_s           /* typedef'd in list.h as 'list' */
{
    struct list_node *head;
    struct list_node *tail;
    size_t num_elems;
};

struct iterator_s       /* typedef'd in list.h as 'iterator' */
{
    struct list_node *curr;
};

bool list_create(list **l)
{
    if (l == NULL)
    {
        return false;
    }

    /* Allocate memory for new list. */
    *l = (list *) malloc(sizeof(list));
    if (*l == NULL)
    {
        return false;
    }

    /* Set default state. */
    (*l)->head = NULL;
    (*l)->tail = NULL;
    (*l)->num_elems = 0;

    return true;
}

bool list_destroy(list **l)
{
    if (l == NULL || *l == NULL)
    {
        return false;
    }

    /* Remove all items from the list. */
    list_clear(*l);

    /* Free the actual list structure. */
    free(*l);
    *l = NULL;

    /* Note that the data contained in the list is NOT freed. */
    /* That task is left up to the client. */

    return true;
}

bool list_empty(const list *l)
{
    return list_size(l) == 0 && l->head == NULL && l->tail == NULL;
}

size_t list_size(const list *l)
{
    if (l == NULL)
    {
        /* Not technically correct, since an invalid list doesn't have a size,
           but it'll have to do. */
        return 0;
    }

    return l->num_elems;
}

bool list_clear(list *l)
{
    if (l == NULL)
    {
        return false;
    }

    struct list_node *curr = l->head;
    struct list_node *prev = NULL;

    /* Iterate across all nodes, freeing each one after visiting. */
    while (curr != NULL)
    {
        prev = curr;
        curr = curr->next;

        free(prev);
    }

    /* Reset list to default state. */
    l->head = NULL;
    l->tail = NULL;
    l->num_elems = 0;

    return true;
}

bool list_append(list *l, const void *entry)
{
    if (l == NULL)
    {
        return false;
    }

    /* Allocate memory for new node. */
    size_t node_size = sizeof(struct list_node);
    struct list_node *n = (struct list_node *) malloc(node_size);
    if (n == NULL)
    {
        return false;
    }

    /* Point the node to its data.
       We're appending to the end of the list, so next should be NULL. */
    n->data = entry;
    n->next = NULL;

    if (l->num_elems == 0)
    {
        /* Set the head and tail to point to the same node because this will
           be the only element in the list. */
        l->head = n;
        l->tail = l->head;
    }
    else
    {
        /* Insert the new node after the tail,
           reassign the tail to point to the new node. */
        l->tail->next = n;
        l->tail = n;
    }

    l->num_elems++;

    return true;
}

bool list_remove(list *l, const void *entry)
{
    if (list_empty(l))
    {
        return false;
    }

    struct list_node *curr = l->head;
    struct list_node *prev = NULL;

    bool removed = false;
    while (curr != NULL && !removed)
    {
        /* Checking for address equality, NOT value equality. */
        if (curr->data != entry)
        {
            /* Element not yet found, move onto next element. */
            prev = curr;
            curr = curr->next;
            continue;
        }

        /* Begin removal process.*/

        if (l->num_elems == 1)
        {
            /* Element is the only element in the list,
               clear both head and tail pointers. */
            l->head = NULL;
            l->tail = NULL;
        }
        else if (curr == l->head)
        {
            /* Element is at the start of the list,
               move head pointer to next element. */
            l->head = l->head->next;
        }
        else if (curr == l->tail)
        {
            /* Element is at the end of the list,
               move tail pointer to previous element. */
            l->tail = prev;
            l->tail->next = NULL;
        }
        else
        {
            /* Element is in between to other elements,
               make 'next' pointer of previous element point to next
               element. */
            prev->next = curr->next;
        }

        free(curr);

        l->num_elems--;
        removed = true;
    }

    return removed;
}

bool iterator_create(const list *l, iterator **it)
{
    if (l == NULL || it == NULL)
    {
        return false;
    }

    /* Allocate memory for iterator. */
    *it = (iterator *) malloc(sizeof(iterator));
    if (*it == NULL)
    {
        return false;
    }

    /* Start the iterator at the list's head. */
    (*it)->curr = l->head;

    return true;
}

bool iterator_destroy(iterator **it)
{
    if (it == NULL || *it == NULL)
    {
        return false;
    }

    free(*it);
    *it = NULL;

    return true;
}

bool iterator_has_next(const iterator *it)
{
    return it != NULL && it->curr != NULL;
}

bool iterator_next(iterator *it, void **entry)
{
    if (!iterator_has_next(it) || entry == NULL)
    {
        return false;
    }

    /* Cast to void * to remove 'const' qualifier,
       data need not be const in client code. */
    *entry = (void *) it->curr->data;

    /* Point to next element in list. */
    it->curr = it->curr->next;

    return true;
}
