/*
* Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
*
* Licensed under the MIT License. See LICENSE at top level directory.
*/

#include "list.h"

list * list_create()
{
    list *l = (list *) malloc(sizeof(list));
    if (l == NULL)
    {
        return NULL;
    }

    l->head = NULL;
    l->tail = NULL;
    return l;
}

bool list_destroy(list *l)
{
    if (l == NULL)
    {
        return false;
    }

    struct list_node *curr = l->head;
    struct list_node *prev = NULL;

    while (curr != NULL)
    {
        prev = curr;
        curr = curr->next;

        free(prev);
    }

    free(l);
    l = NULL;

    return true;
}

bool list_empty(const list *l)
{
    return l == NULL || l->head == NULL || l->tail == NULL;
}

size_t list_size(const list *l)
{
    if (list_empty(l))
    {
        return 0;
    }

    size_t siz = 0;
    struct list_node *curr = l->head;
    while (curr != NULL)
    {
        siz++;
        curr = curr->next;
    }

    return siz;
}

bool list_append(list *l, void *val)
{
    size_t siz = sizeof(struct list_node);
    struct list_node *n = (struct list_node *) malloc(siz);

    n->data = val;
    n->next = NULL;

    if (l == NULL)
    {
        return false;
    }

    if (list_empty(l))
    {
        l->head = n;
        l->tail = l->head;
    }
    else
    {
        l->tail->next = n;
        l->tail = n;
    }

    return true;
}
