/*
* Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
*
* Licensed under the MIT License. See LICENSE at top level directory.
*/

#ifndef _GXTMAKER_LIST_H_
#define _GXTMAKER_LIST_H_

#include <stdbool.h>
#include <stdlib.h>

struct list_node
{
    struct list_node *next;
    const void *data;
};

typedef struct list
{
    struct list_node *head;
    struct list_node *tail;
} list;

list * list_create();

bool list_destroy(list *l);

bool list_empty(const list *l);

size_t list_size(const list *l);

bool list_append(list *l, void *val);

#endif /* _GXTMAKER_LIST_H_ */
