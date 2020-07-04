#include "forces.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const double BUFFER = .01;
const double SPRING_EQUI = 0;
const double SQRT = .5;

aux_t *aux_init(list_t *bodies, double constant){
  aux_t *ans = malloc(sizeof(aux_t));
  assert(ans != NULL);
  ans->bodies = bodies;
  ans->constant = constant;
  return ans;
}

void aux_free(void *ans){
  list_free(((aux_t *)ans)->bodies);
  free((aux_t *)ans);
}

void free_nothing(void *thing){}

void gravity(void *aux){
  aux_t *data = (aux_t *) aux;
  body_t *body_1 = (body_t *) list_get(data->bodies, 0);
  body_t *body_2 = (body_t *) list_get(data->bodies, 1);
  vector_t b1 = body_get_centroid(body_1);
  vector_t b2 = body_get_centroid(body_2);
  double G = data->constant;
  double distance = pow(pow(b2.x-b1.x,2) + pow(b2.y-b1.y,2), SQRT);
  double grav_magnitude = 0.0;
  vector_t diff = vec_subtract(b2, b1);
  diff = vec_multiply(1 / distance, diff);
  if(distance > BUFFER){
    grav_magnitude = G * body_get_mass(body_1) * body_get_mass(body_2) / pow(distance, 2);
  }
  diff = vec_multiply(grav_magnitude, diff);
  vector_t *gravity = malloc(sizeof(vector_t));
  *gravity = diff;
  vector_t *neg_gravity = malloc(sizeof(vector_t));
  *neg_gravity = vec_negate(*gravity);
  body_add_force(body_1, gravity);
  body_add_force(body_2, neg_gravity);
}

void create_newtonian_gravity(scene_t *scene, double G, body_t *body1, body_t *body2) {
  list_t *bodies = list_init(1, (free_func_t)(free_nothing));
  list_add(bodies, body1);
  list_add(bodies, body2);
  aux_t *aux = aux_init(bodies, G);
  scene_add_force_creator(scene, (force_creator_t)gravity, aux, (free_func_t)aux_free);
}

void spring(void *aux){
  aux_t *data = (aux_t *) aux;
  body_t *body_1 = (body_t *) list_get(data->bodies, 0);
  body_t *body_2 = (body_t *) list_get(data->bodies, 1);
  vector_t b1 = body_get_centroid(body_1);
  vector_t b2 = body_get_centroid(body_2);
  double k = data->constant;
  // positive if b1 is higher than b2
  vector_t *force_on_one = malloc(sizeof(vector_t));
  *force_on_one = (vector_t){-k*(b1.x - b2.x), -k*(b1.y - b2.y)};
  vector_t *force_on_two = malloc(sizeof(vector_t));
  *force_on_two = vec_negate(*force_on_one);
  body_add_force(body_1, force_on_one);
  body_add_force(body_2, force_on_two);
}

void create_spring(scene_t *scene, double k, body_t *body1, body_t *body2) {
  list_t *bodies = list_init(1, (free_func_t)(free_nothing));
  list_add(bodies, body1);
  list_add(bodies, body2);
  aux_t *aux = aux_init(bodies, k);
  scene_add_force_creator(scene, (force_creator_t)spring, aux, (free_func_t)aux_free);
}

void drag(void *aux) {
    aux_t *data = (aux_t *)aux;
    body_t *body = (body_t *)list_get(data->bodies, 0);
    double gamma = data->constant;
    vector_t velocity = body_get_velocity(body);
    vector_t *force = malloc(sizeof(vector_t));
    *force = vec_multiply(gamma, vec_negate(velocity));
    body_add_force(body, force);
}

void create_drag(scene_t *scene, double gamma, body_t *body) {
    list_t *bodies = list_init(1, (free_func_t)(free_nothing));
    list_add(bodies, body);
    aux_t *aux = aux_init(bodies, gamma);
    scene_add_force_creator(scene, (force_creator_t)drag, aux, (free_func_t)aux_free);
}
