#include "scene.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

const size_t NUM_BODIES = 25;
const size_t NUM_FORCE_TS = 3 * NUM_BODIES;

typedef struct scene {
    list_t *bodies;
    list_t *forces;
} scene_t;

scene_t *scene_init(void) {
    scene_t *scene = malloc(sizeof(scene_t));
    assert(scene != NULL);
    scene->bodies = list_init(NUM_BODIES, body_free);
    scene->forces = list_init(NUM_FORCE_TS, force_free);
    return scene;
}

void scene_free(scene_t *scene) {
    list_free(scene->bodies);
    list_free(scene->forces);
    free(scene);
}

size_t scene_bodies(scene_t *scene) {
    return list_size(scene->bodies);
}

body_t *scene_get_body(scene_t *scene, size_t index) {
    return (body_t *)list_get(scene->bodies, index);
}

list_t *scene_get_all_bodies(scene_t *scene) {
    return scene->bodies;
}

void scene_add_body(scene_t *scene, body_t *body) {
    list_add(scene->bodies, body);
}

void scene_remove_body(scene_t *scene, size_t index) {
    body_free(list_remove(scene->bodies, index));
}

void scene_add_force_creator(scene_t *scene, force_creator_t forcer, void *aux,
                             free_func_t freer) {
    force_t *force = force_init(forcer, aux, freer);
    list_add(scene->forces, force);
}

force_t *force_init(force_creator_t func, void *aux, free_func_t aux_freer){
  force_t *force = malloc(sizeof(force_t));
  assert(force != NULL);
  force->func = *func;
  force->aux = aux;
  force->aux_free = aux_freer;
  return force;
}

void force_free(void *force){
  free_func_t freer = ((force_t *)force)->aux_free;
  freer(((force_t *)force)->aux);
  free(((force_t *)force));
}

void scene_tick(scene_t *scene, double dt) {
    list_t *forces = scene->forces;
    for(size_t i = 0; i < list_size(scene->forces); i++){
      force_t *force = list_get(forces, i);
      force_creator_t func = force->func;
      void *aux = force->aux;
      func(aux);
    }
    // if body goes off screen, get rid of assoc. forces
    for (size_t i = 0; i < scene_bodies(scene); i++) {
        body_tick(scene_get_body(scene, i), dt);
    }
}
