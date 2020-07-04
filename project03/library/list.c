#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

const int growth_rate = 2;

typedef struct list{
  void **data;
  size_t capacity;
  size_t size;
  freer_t freer;
} list_t;

list_t *list_init(size_t initial_size, freer_t freer){
  list_t *v = malloc(sizeof(list_t));
  assert(v != NULL);
  v->data = malloc(initial_size * sizeof(void *));
  assert(v->data != NULL);
  v->capacity = initial_size;
  v->size = 0;
  v->freer = freer;
  return v;
}

void list_free(list_t *list){
  for(size_t i = 0; i < list_size(list); i++){
    list->freer(list_get(list, i));
  }
  free(list->data);
  free(list);
}

size_t list_size(list_t *list){
  return list->size;
}

size_t list_capacity(list_t *list){
  return list->capacity;
}

void *list_get(list_t *list, size_t index){
  assert(index < list->size && index >= 0);
  void *item = list->data[index];
  return item;
}

void resize(list_t *list){
  void **new_data = malloc(growth_rate * list->capacity * sizeof(void *));
  for(size_t i = 0; i < list->size; i++){
    new_data[i] = list_get(list, i);
  }
  free(list->data);
  list->data = new_data;
  list->capacity *= growth_rate;
}

void list_add(list_t *list, void *value){
  assert(value != NULL);
  if(list->size >= list->capacity){
    resize(list);
  }
  int next_ind = list->size;
  list->data[next_ind] = value;
  list->size++;
}

void collapse(list_t *list, size_t ind){
  for(size_t i = ind; i < list->size - 1; i++){
    list->data[i] = list_get(list, i + 1);
  }
  list->data[list->size - 1] = (void *)NULL;
}

void *list_remove(list_t *list, size_t ind){
  assert (ind >= 0);
  assert(ind < list_capacity(list));
  void *temp = list->data[ind];
  collapse(list, ind);
  list->size--;
  return temp;
}
