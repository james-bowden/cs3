#include "polygon.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


double polygon_area(vec_list_t *polygon){
  double area = 0.0;
  int num_vert = sizeof(polygon);
  // j will serve as "previous" vertex
  int j = num_vert - 1;
  for(int i = 0; i < num_vert; i++){
    area += ((vec_list_get(polygon, j)->x + vec_list_get(polygon, i)->x) * (vec_list_get(polygon, j)->y - vec_list_get(polygon, i)->y));
    j = i;
  }
  return (double)fabs(area / 2.0);
}

vector_t polygon_centroid(vec_list_t *polygon){
  double area = polygon_area(polygon);
  double center_x = 0.0;
  double center_y = 0.0;
  for (int i = 0; i < sizeof(polygon); i++){
    center_x += (vec_list_get(polygon, i)->x + vec_list_get(polygon, i + 1)->x) *
        ((vec_list_get(polygon, i)->x * vec_list_get(polygon, i + 1)->y) - (vec_list_get(polygon, i + 1)->x * vec_list_get(polygon, i)->y));
    center_y = (vec_list_get(polygon, i)->y + vec_list_get(polygon, i + 1)->y) *
        ((vec_list_get(polygon, i)->x * vec_list_get(polygon, i + 1)->y) - (vec_list_get(polygon, i + 1)->x * vec_list_get(polygon, i)->y));
  }
  center_x *= (1 / 6) / area;
  center_y *= (1 / 6) / area;
  vector_t center = {center_x, center_y};
  return center;
}

void polygon_translate(vec_list_t *polygon, vector_t translation){
  for(size_t i = 0; i < sizeof(polygon); i++){
    *vec_list_get(polygon, i) = vec_add(translation, *vec_list_get(polygon, i));
  }
}

void polygon_rotate(vec_list_t *polygon, double angle, vector_t point){
  vector_t center_old = polygon_centroid(polygon);
  // rotate center about point
  vector_t center_new = vec_subtract(center_old, point);
  // subtraction gives vector from origin, where origin = point
  center_new = vec_rotate(center_new, angle);
  // add new center to point
  center_new = vec_add(point, center_new);
  // find translation difference btwn new and old centers
  vector_t difference = vec_subtract(center_new, center_old);
  // translate all vectors by difference
  polygon_translate(polygon, difference);
}
