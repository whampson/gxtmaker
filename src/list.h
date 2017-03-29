/*
* Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
*
* Licensed under the MIT License. See LICENSE at top level directory.
*/

#ifndef _GXTMAKER_LIST_H_
#define _GXTMAKER_LIST_H_

#include <stdbool.h>
#include <stdlib.h>

typedef struct list_s list;
typedef struct iterator_s iterator;

/**
 * Creates a new list on the heap.
 */
bool list_create(list **l);

bool list_destroy(list **l);

bool list_empty(const list *l);

size_t list_size(const list *l);

bool list_clear(list *l);

bool list_append(list *l, const void *val);

bool list_remove(list *l, const void *val);

bool iterator_create(const list *l, iterator **it);

bool iterator_destroy(iterator **it);

bool iterator_has_next(const iterator *it);

bool iterator_next(iterator *it, void **val);

#endif /* _GXTMAKER_LIST_H_ */
