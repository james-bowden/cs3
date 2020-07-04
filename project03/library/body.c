#include "body.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct body{
  polygon_t *shape;
  vector_t velocity;
  rgb_color_t color;
  double orientation;
  double mass;
} body_t;

body_t *body_init(list_t *shape, double mass, rgb_color_t color){
  body_t *body = malloc(sizeof(body_t));
  assert(body != NULL);
  body->shape = polygon_init(list_size(shape));
  polygon_set_points(body->shape, shape);
  body->velocity = (vector_t){0.0, 0.0};
  body->color = color;
  body->mass = mass;
  body->orientation = 0;
  return body;
}

void body_free(void *body){
  polygon_free(((body_t *)body)->shape);
  free(((body_t *)body));
}

list_t *body_get_shape(body_t *body){
  list_t *points = polygon_get_points(body->shape);
  size_t size = list_size(points);
  list_t *copy = list_init(size, vec_free);
  for(size_t i = 0; i < size; i++){
    vector_t *temp = malloc(sizeof(vector_t));
    *temp = *(vector_t *)list_get(points, i);
    list_add(copy, temp);
  }
  return copy;
}

vector_t body_get_centroid(body_t *body){
  return polygon_centroid(body->shape);
}

vector_t body_get_velocity(body_t *body){
  return body->velocity;
}

rgb_color_t body_get_color(body_t *body){
  return body->color;
}

void body_set_centroid(body_t *body, vector_t x){
  vector_t old_centroid = body_get_centroid(body);
  polygon_translate(body->shape, vec_subtract(x, old_centroid));
}

void body_set_velocity(body_t *body, vector_t v){
  body->velocity = v;
}

void body_set_rotation(body_t *body, double angle){
  polygon_rotate(body->shape, angle - body->orientation, body_get_centroid(body));
  body->orientation = angle;
}

void body_translate(body_t *body, vector_t diff){
  polygon_translate(body->shape, diff);
}

void body_tick(body_t *body, double dt){
  polygon_translate(body->shape, vec_multiply(dt, body->velocity));
}
