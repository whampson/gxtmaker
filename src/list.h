/*
* Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
*
* Licensed under the MIT License. See LICENSE at top level directory.
*/

#ifndef _GXTMAKER_LIST_H_
#define _GXTMAKER_LIST_H_

#include <stdbool.h>
#include <stdlib.h>

typedef struct list_internal list;

list * list_create(void);

bool list_destroy(list *l);

bool list_empty(const list *l);

size_t list_size(const list *l);

bool list_append(list *l, void *val);

#endif /* _GXTMAKER_LIST_H_ */
