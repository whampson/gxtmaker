/*
 * Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
 *
 * Licensed under the MIT License. See LICENSE at top level directory.
 */

/**
 * This file contains definitions for a simple list container and list
 * iterator.
 */

#ifndef _GXTMAKER_LIST_H_
#define _GXTMAKER_LIST_H_

#include <stdbool.h>
#include <stdlib.h>

typedef struct list_s list;
typedef struct iterator_s iterator;

/**
 * Creates an empty list.
 *
 * This function is to be called before using any other list function.
 * list_destroy() should be called when the list is no longer needed.
 *
 * @param l a pointer to the list to be created
 *
 * @return true  if the list was created successfully
 *         false if the list could not be created
 *               (e.g. due to lack of available memory)
 */
bool list_create(list **l);

/**
 * Deletes an existing list.
 *
 * NOTE: This function does NOT free memory associated with data stored within
 * the list; only the list itself is freed. Data within the list, if allocated
 * on the heap, must be freed manually.
 *
 * @param l a pointer to the list to be deleted
 *
 * @return true  if the list was successfully freed
 *         false if no memory was freed
 *               (e.g. if the provided list was never initialized)
 */
bool list_destroy(list **l);

/**
 * Checks whether a given list contains elements.
 *
 * @param l the list to check for emptiness
 *
 * @return true  if the list has no elements (or if the list is uninitialized)
 *         false if the list contains elements
 */
bool list_empty(const list *l);

/**
 * Gets the number of elements in a given list.
 *
 * @param l the list to get the size of
 *
 * @return the number of elements in the list (0 if the list is uninitialized)
 */
size_t list_size(const list *l);

/**
 * Removes all elements from a given list.
 *
 * @param l the list to clear
 *
 * @return true  if the list was successfully cleared
 *         false if the list could not be cleared
 *               (e.g. due to the list being uninitialized)
 */
bool list_clear(list *l);

/**
 * Adds an entry to a given list by appending it to the rear.
 *
 * @param l     the list to append
 * @param entry a pointer to the entry to be appended
 *
 * @return true  if the entry was successfully added to the list
 *         false if the entry could not be added
 *               (e.g. due to the list being uninitialized)
 */
bool list_append(list *l, const void *entry);

/**
 * Removes an entry from a given list.
 *
 * NOTE: This function removes the entry by checking it's address, not the
 * underlying value. This means, for example, if the list contains the
 * characters 'a', 'b', and 'c', the second element ('b') cannot be removed by
 * simply passing some arbitrary pointer to the character 'b' in for the 'val'
 * parameter -- 'val' must be a pointer to the same address that was used in
 * list_append().
 *
 * @param l     the list to remove from
 * @param entry a pointer to the entry to be removed
 *
 * @return true  if the entry was successfully removed from the list
 *         false if no removal occured, e.g. due to the entry not existing in
 *               the list or the list being uninitialized
 */
bool list_remove(list *l, const void *entry);

bool iterator_create(const list *l, iterator **it);

bool iterator_destroy(iterator **it);

bool iterator_has_next(const iterator *it);

bool iterator_next(iterator *it, void **val);

#endif /* _GXTMAKER_LIST_H_ */
