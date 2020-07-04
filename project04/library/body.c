#include "body.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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
    return body;
}

void body_free(void *body) {
    polygon_free(((body_t *)body)->shape);
    list_free(((body_t *)body)->forces);
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
    return polygon_centroid(body->shape);
}

vector_t body_get_velocity(body_t *body) {
    return body->velocity;
}

rgb_color_t body_get_color(body_t *body) {
    return body->color;
}

void body_set_centroid(body_t *body, vector_t x) {
    vector_t old_centroid = body_get_centroid(body);
    polygon_translate(body->shape, vec_subtract(x, old_centroid));
}

void body_set_velocity(body_t *body, vector_t v) {
    body->velocity = v;
}

void body_set_rotation(body_t *body, double angle) {
    polygon_rotate(body->shape, angle - body->orientation, body_get_centroid(body));
    body->orientation = angle;
}

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
    vector_t vel_new = velocity;
    for(size_t i = 0; i < list_size(body->forces); i++){
      vector_t force = *(vector_t *)list_get(body->forces, i);
      // f = ma
      vector_t accel = vec_multiply(1 / body->mass, force);
      vector_t dv = vec_multiply(dt, accel);
      vel_new = vec_add(vel_new, dv);
    }
    vector_t vel_avg = vec_multiply(AVG, vec_add(velocity, vel_new));
    polygon_translate(body->shape, vec_multiply(dt, vel_avg));
    body_set_velocity(body, vel_new);
    list_clear(body->forces);
}
