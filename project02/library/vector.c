
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

const vector_t VEC_ZERO = {0.0, 0.0};

void vec_free(void *x){
  free((vector_t *)x);
}

vector_t vec_add(vector_t v1, vector_t v2){
  vector_t vec = {v1.x + v2.x, v1.y + v2.y};
  return vec;
}

vector_t vec_subtract(vector_t v1, vector_t v2){
  vector_t vec = {v1.x - v2.x, v1.y - v2.y};
  return vec;
}

vector_t vec_negate(vector_t v){
  vector_t vec = {-1.0 * v.x, -1.0 * v.y};
  return vec;
}

vector_t vec_multiply(double scalar, vector_t v){
  vector_t vec = {scalar * v.x, scalar * v.y};
  return vec;
}

double vec_dot(vector_t v1, vector_t v2){
  return ((v1.x * v2.x) + (v1.y * v2.y));
}

double vec_cross(vector_t v1, vector_t v2){
  return (v1.x * v2.y) - (v1.y * v2.x);
}

vector_t vec_rotate(vector_t v, double angle){
  // multiply by rotation matrix
  double x = v.x * cos(angle) - v.y * sin(angle);
  double y = (v.x * sin(angle)) + (v.y * cos(angle));
  vector_t vec = {x, y};
  return vec;
}
