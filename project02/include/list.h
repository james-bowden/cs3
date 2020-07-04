#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h>

/**
 * A growable array of any type, stored as pointers to malloc()ed void*s.
 * A list owns all the pointers in it, so it is responsible for free()ing them.
 * Note that all elements of a given instance of list will be void *s, so
 * they must be casted to whatever object type they are of whenever taken out
 * (e.g. via the get, remove methods).
 * This line does two things:
 * - Declares a "struct list" type
 * - Makes "list_t" an alias for "struct list"
 *
 * You will need to implement this struct type in list.c.
 */
typedef struct list list_t;

typedef void (*freer_t)(void *x);

/**
 * Allocates memory for a new list with space for the given number of elements.
 * The list is initially empty.
 * Asserts that the required memory was allocated.
 *
 * @param initial_size the number of list elements to allocate space for
 * @return a pointer to the newly allocated list
 */
list_t *list_init(size_t initial_size);

/**
 * Releases the memory allocated for a list.
 * Also frees all items in the list.
 *
 * @param list a pointer to a list returned from list_init()
 * @param function that frees element type in list
 */
void list_free(list_t *list, freer_t freer);

/**
 * Gets the size of a list (the number of occupied elements).
 * Note that this is NOT the list's capacity.
 *
 * @param list a pointer to a list returned from list_init()
 * @return the number of items in the list
 */
size_t list_size(list_t *list);

/**
 * Gets the capacity of a list (the number of spaces in array).
 * Primarily here for testing purposes, as capacity is relevant when
 * the list must be resized.
 *
 * @param list a pointer to a list returned from list_init()
 * @return the number of spaces in the list
 */
size_t list_capacity(list_t *list);

/**
 * Gets the element at a given index in a list.
 * Asserts that the index is valid, given the list's current size.
 *
 * @param list a pointer to a list returned from list_init()
 * @param index an index in the list (the first element is at 0)
 * @return the item at the given index
 */
void *list_get(list_t *list, size_t index);

/**
 * Doubles the current capacity of list->data to make space for new
 * elements. Copies over elements that were already added.
 *
 * @param list a pointer to a list returned from list_init()
 */
void resize(list_t *list);

/**
 * Appends an element to the end of a list.
 * Asserts that the value being added is not NULL.
 * If list is getting full, increases capacity to fit new elements.
 *
 * @param list a pointer to a list returned from list_init()
 * @param value the item to add to the end of the list
 */
void list_add(list_t *list, void *value);

/**
 * Removes the element at the end of a list and returns it.
 * Asserts that the list is nonempty.
 *
 * @param list a pointer to a list returned from list_init()
 * @return the item at the end of the list
 */
void *list_remove(list_t *list);

#endif // #ifndef __lIST_H__
