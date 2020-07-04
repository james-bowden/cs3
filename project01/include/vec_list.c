#include "vec_list.h"
#include "vector.c"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


typedef struct vec_list{
  vector_t **data;
} vec_list_t;

vec_list_t *vec_list_init(size_t initial_size){
  vec_list_t *v = malloc(sizeof(vec_list_t));
  v->data = malloc(initial_size);
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
  size_t size = 0;
  // count # null elements
  for(size_t i = 0; i < sizeof(list->data); i++){
    if(list->data[i] != NULL){
      size++;
    }
  }
  return size;
}

vector_t *vec_list_get(vec_list_t *list, size_t index){
  // check the index is within the list
  if(index >= sizeof(list->data))
  {
    exit(1);
  }
  // get vector at index in list
  vector_t *vec = list->data[index];
  return vec;
}

void vec_list_add(vec_list_t *list, vector_t *value){
  // assert value is not NULL
  if(value == NULL){
    exit(1);
  }
  int next_ind = vec_list_size(list);
  if(next_ind < sizeof(list->data)){
    list->data[next_ind] = value;
  }
  else{
    // double the size of the array if needs more space
    vector_t **new_data = malloc(sizeof(list->data) * 2);
    // copy all elements over
    for(size_t i = 0; i < next_ind; i++){
      new_data[i] = list->data[i];
    }
    // add in value
    new_data[next_ind] = value;
    // change out arrays
    free(list->data);
    list->data = new_data;
  }
}

vector_t *vec_list_remove(vec_list_t *list){
  int last_ind = vec_list_size(list) - 1;
  // check if list is empty, exit if is
  if(last_ind < 0){
    exit(2);
  }
  vector_t *temp = list->data[last_ind];
  list->data[last_ind] = NULL;
  return temp;
}
