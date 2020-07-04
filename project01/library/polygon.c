#include "polygon.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


double polygon_area(vec_list_t *polygon){
  double area = 0.0;
  size_t num_vert = vec_list_size(polygon);
  // j will serve as "previous" vertex
  size_t j = num_vert - 1;
  for(size_t i = 0; i < num_vert; i++){
    area += ((vec_list_get(polygon, j)->x * vec_list_get(polygon, i)->y) -
    (vec_list_get(polygon, j)->y * vec_list_get(polygon, i)->x));
    // keep one behind i
    j = i;
  }
  return fabs(area / 2.0);
}

vector_t polygon_centroid(vec_list_t *polygon){
  double center_x = 0.0;
  double center_y = 0.0;
  size_t num_vert = vec_list_size(polygon);
  // j will serve as "previous" vertex
  size_t j = num_vert - 1;
  for (size_t i = 0; i < num_vert; i++){
    // calculate this first bc it's common factor for both x and y coordinates
    double comm_f = (vec_list_get(polygon, j)->x * vec_list_get(polygon, i)->y)
    - (vec_list_get(polygon, i)->x * vec_list_get(polygon, j)->y);
    center_x += (vec_list_get(polygon, j)->x + vec_list_get(polygon, i)->x) *
    comm_f;
    center_y += (vec_list_get(polygon, j)->y + vec_list_get(polygon, i)->y) *
    comm_f;
    // keep one behind i
    j = i;
  }
  double area = polygon_area(polygon);
  center_x /= (6.0 * area);
  center_y /= (6.0 * area);
  vector_t center = {center_x, center_y};
  return center;
}

void polygon_translate(vec_list_t *polygon, vector_t translation){
  for(size_t i = 0; i < vec_list_size(polygon); i++){
    *vec_list_get(polygon, i) = vec_add(translation, *vec_list_get(polygon, i));
  }
}

void polygon_rotate(vec_list_t *polygon, double angle, vector_t point){
  for(size_t i = 0; i < vec_list_size(polygon); i++){
    // get original vector
    vector_t old = *vec_list_get(polygon, i);
    // rotate center about point (subtraction gives vector from origin, where
    // origin = point)
    vector_t new = vec_subtract(old, point);
    new = vec_rotate(new, angle);
    // add new vector to point
    new = vec_add(point, new);
    *vec_list_get(polygon, i) = new;
  }
}
