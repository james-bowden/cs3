#include "polygon.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

typedef struct polygon{
  list_t *points;
  float *colors;
  vector_t *velocity;
} polygon_t;

polygon_t *polygon_init(size_t num_pts){
  polygon_t *p = malloc(sizeof(polygon_t));
  assert(p != NULL);
  p->points = list_init(num_pts);
  assert(p->points != NULL);
  p->colors = malloc(3 * sizeof(float));
  assert(p->colors != NULL);
  p->velocity = malloc(sizeof(vector_t));
  assert(p->velocity != NULL);
  return p;
}

void polygon_free(void *polygon){
  list_free(((polygon_t *)polygon)->points, vec_free);
  free(((polygon_t *)polygon)->colors);
  free(((polygon_t *)polygon)->velocity);
  free(((polygon_t *)polygon));
}

void set_points(polygon_t *polygon, list_t *points){
  polygon->points = points;
}

void set_velocity(polygon_t *polygon, vector_t *velocity){
  polygon->velocity->x = velocity->x;
  polygon->velocity->y = velocity->y;
}

void set_colors(polygon_t *polygon, float red, float green, float blue){
  polygon->colors[0] = red;
  polygon->colors[1] = green;
  polygon->colors[2] = blue;
}

void set_all(polygon_t *polygon, list_t *points, vector_t *velocity,
  float r, float g, float b){
    set_points(polygon, points);
    set_velocity(polygon, velocity);
    set_colors(polygon, r, g, b);
}

float get_r(polygon_t *polygon){
  return polygon->colors[0];
}

float get_g(polygon_t *polygon){
  return polygon->colors[1];
}

float get_b(polygon_t *polygon){
  return polygon->colors[2];
}

list_t *get_points(polygon_t *polygon){
  return polygon->points;
}

vector_t *get_velocity(polygon_t *polygon){
  return polygon->velocity;
}

double polygon_area(polygon_t *polygon){
  double area = 0.0;
  list_t *points = polygon->points;
  size_t num_vert = list_size(points);
  // j will serve as "previous" vertex
  size_t j = num_vert - 1;
  for(size_t i = 0; i < num_vert; i++){
    area += ((((vector_t *)list_get(points, j))->x *
    ((vector_t *)list_get(points, i))->y) -
    (((vector_t *)list_get(points, j))->y *
    ((vector_t *)list_get(points, i))->x));
    // keep one behind i
    j = i;
  }
  return fabs(area / 2.0);
}

vector_t polygon_centroid(polygon_t *polygon){
  double center_x = 0.0;
  double center_y = 0.0;
  list_t *points = polygon->points;
  size_t num_vert = list_size(points);
  // j will serve as "previous" vertex
  size_t j = num_vert - 1;
  for (size_t i = 0; i < num_vert; i++){
    // calculate this first bc it's common factor for both x and y coordinates
    double comm_f = (((vector_t *)list_get(points, j))->x *
    ((vector_t *)list_get(points, i))->y)-
    (((vector_t *)list_get(points, i))->x *
    ((vector_t *)list_get(points, j))->y);
    center_x += (((vector_t *)list_get(points, j))->x +
    ((vector_t *)list_get(points, i))->x) * comm_f;
    center_y += (((vector_t *)list_get(points, j))->y +
    ((vector_t *)list_get(points, i))->y) * comm_f;
    // keep one behind i
    j = i;
  }
  double area = polygon_area(polygon);
  center_x /= (6.0 * area);
  center_y /= (6.0 * area);
  vector_t center = {center_x, center_y};
  return center;
}

void polygon_translate(polygon_t *polygon, vector_t translation){
  list_t *points = polygon->points;
  for(size_t i = 0; i < list_size(points); i++){
    *(vector_t *)list_get(points, i) = vec_add(translation,
      *(vector_t *)list_get(points, i));
  }
}

void polygon_rotate(polygon_t *polygon, double angle, vector_t point){
  list_t *points = polygon->points;
  for(size_t i = 0; i < list_size(points); i++){
    vector_t old = *(vector_t *)list_get(points, i);
    // rotate center about point (subtraction gives vector from origin, where
    // origin = point)
    vector_t new = vec_subtract(old, point);
    new = vec_rotate(new, angle);
    // add new vector to point
    new = vec_add(point, new);
    *(vector_t *)list_get(points, i) = new;
  }
}

void polygon_scale(double mult, polygon_t *polygon){
  list_t *points = polygon->points;
  for(size_t i = 0; i < list_size(points); i++){
    vector_t current = *(vector_t *)list_get(points, i);
    current = vec_multiply(mult, current);
  }
}
