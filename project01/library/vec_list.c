#include "vec_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>


typedef struct vec_list{
  vector_t **data;
  size_t capacity;
  size_t size;
} vec_list_t;

vec_list_t *vec_list_init(size_t initial_size){
  vec_list_t *v = malloc(sizeof(vec_list_t));
  v->data = malloc(initial_size * sizeof(vector_t));
  v->capacity = initial_size;
  v->size = 0;
  return v;
}

void vec_list_free(vec_list_t *list){
  // free each element of the array
  for(size_t i = 0; i < vec_list_size(list); i++){
    free(vec_list_get(list, i));
  }
  free(list->data);
  free(list);
}

size_t vec_list_size(vec_list_t *list){
  return list->size;
}

vector_t *vec_list_get(vec_list_t *list, size_t index){
  // check the index is within the list
  assert(index < list->size && index >= 0);
  // get vector at index in list, ok if null
  vector_t *vec = list->data[index];
  return vec;
}

void vec_list_add(vec_list_t *list, vector_t *value){
  // assert value is not NULL
  assert(value != NULL);
  // make sure there is space left in array
  assert(list->size < list->capacity);
  // put value in next index, at end
  int next_ind = list->size;
  list->data[next_ind] = value;
  // increment size
  list->size++;
}

vector_t *vec_list_remove(vec_list_t *list){
  int last_ind = vec_list_size(list) - 1;
  // assert list is nonempty
  assert (last_ind >= 0);
  vector_t *temp = list->data[last_ind];
  list->data[last_ind] = NULL;
  list->size--;
  return temp;
}
