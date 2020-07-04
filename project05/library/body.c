#include <math.h>
#include "body.h"

const int NUM_FORCES = 50;
const double AVG = .5;

typedef struct body {
    polygon_t *shape;
    vector_t velocity;
    rgb_color_t color;
    double orientation;
    double mass;
    list_t *forces;
    list_t *impulses;
    void *info;
    bool has_info;
    bool rem;
    vector_t centroid;
} body_t;

body_t *body_init(list_t *shape, double mass, rgb_color_t color) {
    body_t *body = malloc(sizeof(body_t));
    assert(body != NULL);
    body->shape = polygon_init(list_size(shape));
    polygon_set_points(body->shape, shape);
    body->velocity = (vector_t){0.0, 0.0};
    body->color = color;
    body->mass = mass;
    body->orientation = 0;
    body->forces = list_init(NUM_FORCES, vec_free);
    body->rem = false;
    // get init centroid and move centroid back over to 0
    vector_t temp_c = polygon_centroid(body->shape);
    temp_c = vec_negate(temp_c);
    polygon_translate(body->shape, temp_c);
    body->centroid = (vector_t){0.0, 0.0};
    body->has_info = false;
    return body;
}

body_t *body_init_info(list_t *shape, double mass, rgb_color_t color, void *aux) {
    body_t *body = malloc(sizeof(body_t));
    assert(body != NULL);
    body->shape = polygon_init(list_size(shape));
    polygon_set_points(body->shape, shape);
    body->velocity = (vector_t){0.0, 0.0};
    body->color = color;
    body->mass = mass;
    body->orientation = 0;
    body->forces = list_init(NUM_FORCES, vec_free);
    body->rem = false;
    body->info = aux;
    body->has_info = true;
    // get init centroid and move centroid back over to 0
    vector_t temp_c = polygon_centroid(body->shape);
    temp_c = vec_negate(temp_c);
    polygon_translate(body->shape, temp_c);
    // assert(polygon_centroid(body->shape).x == 0.0);
    // assert(polygon_centroid(body->shape).y == 0.0);
    body->centroid = (vector_t){0.0, 0.0};
    return body;
}

void *body_get_info(body_t *body){
  return body->info;
}

void body_free(void *body) {
    polygon_free(((body_t *)body)->shape);
    list_free(((body_t *)body)->forces);
    if(((body_t *)body)->has_info){
      // assuming is string here!!
      free(body_get_info(body));
    }
    free(((body_t *)body));
}

double body_get_mass(body_t *body){
  return body->mass;
}

list_t *body_get_shape(body_t *body) {
    list_t *points = polygon_get_points(body->shape);
    size_t size = list_size(points);
    list_t *copy = list_init(size, vec_free);
    for (size_t i = 0; i < size; i++) {
        vector_t *temp = malloc(sizeof(vector_t));
        *temp = *(vector_t *)list_get(points, i);
        list_add(copy, temp);
    }
    return copy;
}

vector_t body_get_centroid(body_t *body) {
    // return polygon_centroid(body->shape);
    return body->centroid;
}

vector_t body_get_velocity(body_t *body) {
    return body->velocity;
}

rgb_color_t body_get_color(body_t *body) {
    return body->color;
}

void body_set_centroid(body_t *body, vector_t x) {
    // vector_t old_centroid = body_get_centroid(body);
    polygon_translate(body->shape, vec_subtract(x, body->centroid));
    body->centroid = x;
}

void body_set_velocity(body_t *body, vector_t v) {
    body->velocity = v;
}

void body_set_rotation(body_t *body, double angle) {
    polygon_rotate(body->shape, angle - body->orientation, body_get_centroid(body));
    body->orientation = angle;
}

// whoops deprecated
void body_translate(body_t *body, vector_t diff) {
    polygon_translate(body->shape, diff);
}

void body_add_force(body_t *body, vector_t *force) {
  list_add(body->forces, force);
}

void body_add_impulse(body_t *body, vector_t *impulse) {
}

void body_tick(body_t *body, double dt) {
    vector_t velocity = body->velocity;
    vector_t vel_new = body->velocity;
    for(size_t i = 0; i < list_size(body->forces); i++){
      vector_t force = *(vector_t *)list_get(body->forces, i);
      // f = ma
      vector_t accel = vec_multiply(1.0 / body->mass, force);
      vector_t dv = vec_multiply(dt, accel);
      vel_new = vec_add(vel_new, dv);
    }
    vector_t vel_avg = vec_multiply(AVG, vec_add(velocity, vel_new));
    vector_t trans = (vector_t)vec_multiply(dt, vel_avg);
    vector_t new_c = vec_add(body->centroid, trans);
    body_set_centroid(body, new_c);
    body_set_velocity(body, vel_new);
    list_clear(body->forces);
}

void body_remove(body_t *body){
  body->rem = true;
}

bool body_is_removed(body_t *body){
  return body->rem;
}

bool body_test_bounding_box(body_t *body_one, body_t *body_two){
  double min_x_1, min_x_2, min_y_1, min_y_2, max_x_1, max_x_2, max_y_1, max_y_2;
  min_x_1 = min_x_2 = min_y_1 = min_y_2 = INFINITY;
  max_x_1 = max_x_2 = max_y_1 = max_y_2 = -INFINITY;
  list_t *temp1 = polygon_get_points(body_one->shape);
  list_t *temp2 = polygon_get_points(body_two->shape);
  for(size_t i = 0; i < list_size(temp1); ++i){
    double x = ((vector_t *)list_get(temp1, i))->x;
    double y = ((vector_t *)list_get(temp1, i))->y;
    if(x < min_x_1){
      min_x_1 = x;
    } else if (x > max_x_1){
      max_x_1 = x;
    }
    if(y < min_y_1){
      min_y_1 = y;
    } else if (y > max_y_1){
      max_y_1 = y;
    }
  }
  for(size_t i = 0; i < list_size(temp2); ++i){
    double x = ((vector_t *)list_get(temp2, i))->x;
    double y = ((vector_t *)list_get(temp2, i))->y;
    if(x < min_x_2){
      min_x_2 = x;
    } else if (x > max_x_2){
      max_x_2 = x;
    }
    if(y < min_y_2){
      min_y_2 = y;
    } else if (y > max_y_2){
      max_y_2 = y;
    }
  }
  if((min_x_1 < min_x_2 && min_x_2 < max_x_1) || (min_x_2 < min_x_1 && min_x_1 < max_x_2)){
    if((min_y_1 < min_y_2 && min_y_2 < max_y_1) ||
        (min_y_2 < min_y_1 && min_y_1 < max_y_2)){
      return true;
    }
  }
  return false;
}


bool body_is_colliding(body_t *body_one, body_t *body_two){
  if(!body_test_bounding_box(body_one, body_two)){
    return false;
  }
  else{
    list_t *all_perpendicular = list_init(list_size(polygon_get_points(body_one->shape))+
      list_size(polygon_get_points(body_two->shape)), vec_free);
    int counter = 0;
    for(size_t i = 0; i < list_size(polygon_get_points(body_one->shape)); ++i){
      vector_t *vec_one = (vector_t *) list_get(polygon_get_points(body_one->shape), i);
      vector_t *perp = malloc(sizeof(vector_t));
      *perp = (vector_t){-vec_one->y, vec_one->x};
      double distance = pow(pow(vec_one->x, 2) + pow(vec_one->y, 2), 0.5);
      if(distance != 0)
        *perp = vec_multiply(1/distance, *perp);
      list_add(all_perpendicular, perp);
      counter++;
    }
    for(size_t i = 0; i < list_size(polygon_get_points(body_two->shape)); ++i){
      vector_t *vec_two = (vector_t *) list_get(polygon_get_points(body_two->shape), i);
      vector_t *perp = malloc(sizeof(vector_t));
      *perp = (vector_t){-vec_two->y, vec_two->x};
      double distance = pow(pow(vec_two->x, 2) + pow(vec_two->y, 2), 0.5);
      if(distance != 0)
        *perp = vec_multiply(1/distance, *perp);
      list_add(all_perpendicular, perp);
    }
    // Compare the points of each polygon to the unit vectors (get closest, farthest)
    for(size_t i = 0; i < list_size(all_perpendicular); ++i){
      double max_scale = -INFINITY;
      double min_scale = INFINITY;
      vector_t *side = (vector_t *) list_get(all_perpendicular, i);
      // Checks the first shape
      list_t *p1 = polygon_get_points(body_one->shape);
      for(size_t j = 0; j < list_size(p1); ++j){
        vector_t *vec_one = (vector_t *) list_get(p1, j);
        double scale = vec_dot(*side, *vec_one);
        if(scale > max_scale)
          max_scale = scale;
        if(scale < min_scale)
          min_scale = scale;
      }
      // Checks the second shape
      list_t *p2 = polygon_get_points(body_two->shape);
      double max_scale_two = -INFINITY;
      double min_scale_two = INFINITY;
      for(size_t j = 0; j < list_size(p2); ++j){
        vector_t *vec_one = (vector_t *) list_get(p2, j);
        double scale = vec_dot(*side, *vec_one);
        if(scale > max_scale_two)
          max_scale_two = scale;
        if(scale < min_scale_two)
          min_scale_two = scale;
      }
      if(!(min_scale_two < max_scale && min_scale < max_scale_two)){
        list_free(all_perpendicular);
        return false;
      }
    }
    list_free(all_perpendicular);
    return true;
  }
}
