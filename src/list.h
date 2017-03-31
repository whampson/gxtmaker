/*
 * Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
 *
 * Licensed under the MIT License. See LICENSE at top level directory.
 */

/**
 * Declarations for a simple list container and list iterator.
 *
 * list:
 * The 'list' type may be used to arrange elements of any type in a linear,
 * sequential fashion. It is recommended that all elements in the list be of
 * the same type, but not a requirement.
 *
 * The 'list' type is mutable. This is because the list only contains pointers
 * to elements, not the elements themselves. This means that if a given element
 * is modified somewhere in the program, the modification will also be present
 * in the list.
 *
 * iterator:
 * The 'iterator' type is used for traversing across all elements of a given
 * list. Iterators can only traverse a list once. If a list needs to be
 * traversed multiple times, multiple iterators need to be created (one for
 * each traversal).
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
 *               (e.g. due to the list being uninitialized
 *               or lack of available memory)
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

/**
 * Creates a new list iterator for traversing an existing list.
 *
 * This function is to be called before using any other iterator function.
 * iterator_destroy() should be called when the list is no longer needed.
 *
 * @param l  the list to be traversed by the iterator
 * @param it a pointer to the iterator to be created
 *
 * @return true  if the iterator was created successfully
 *         false if the iterator could not be created
 *               (e.g. due to lack of available memory)
 */
bool iterator_create(const list *l, iterator **it);

/**
* Deletes an existing list iterator.
*
* @param l a pointer to the iterator to be deleted
*
* @return true  if the iterator was successfully freed
*         false if no memory was freed
*               (e.g. if the provided iterator was never initialized)
*/
bool iterator_destroy(iterator **it);

/**
 * Checks whether a given iteration has more elements.
 *
 * @param it the iterator to check 
 */
bool iterator_has_next(const iterator *it);

/**
 * Gets the next element in a given iteration.
 *
 * It is recommended to call iterator_has_next() before using this function.
 *
 * @param it    the current iteration
 * @param entry a pointer to the address where the retrieved item should be
 *              stored
 *
 * @return true  if an item was retrieved
 *         false if no item was retrieved
 *               (e.g. due to the iteration containing no more elements)
 */
bool iterator_next(iterator *it, void **entry);

#endif /* _GXTMAKER_LIST_H_ */
