#include "scene.h"

const size_t NUM_BODIES = 50;
const size_t NUM_FORCE_TS = 1; //3 * NUM_BODIES;

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
    // body_free(list_remove(scene->bodies, index)); // now in tick
    body_t *body = scene_get_body(scene, index);
    body_remove(body);
    list_remove(scene->bodies, index);
    body_free(body);
}

void scene_add_force_creator(scene_t *scene, force_creator_t forcer, void *aux,
                             free_func_t freer) {
    // force_t *force = force_init(forcer, aux, freer);
    // list_add(scene->forces, force);
    // scene_add_bodies_force_creator(scene, forcer, aux, ((aux_t *)aux)->bodies, freer);
    // above line can't really work bc aux_t not defined here, so cannot get bodies out of it
    exit(6);
}

void scene_add_bodies_force_creator(
    scene_t *scene,
    force_creator_t forcer,
    void *aux,
    list_t *bodies,
    free_func_t freer
){
  force_t *force = force_init(forcer, aux, bodies, freer);
  list_add(scene->forces, force);
}

force_t *force_init(force_creator_t func, void *aux, list_t *bodies, free_func_t aux_freer){
  force_t *force = malloc(sizeof(force_t));
  assert(force != NULL);
  force->func = *func;
  force->aux = aux;
  force->aux_free = aux_freer;
  force->bodies = bodies;
  return force;
}

void force_free(void *force){
  free_func_t freer = ((force_t *)force)->aux_free;
  freer(((force_t *)force)->aux);
  free(((force_t *)force));
  // shouldn't free bodies
}

bool force_is_removed(force_t *force, body_t *rem){
  list_t *f = force->bodies;
  for(size_t i = 0; i < list_size(f); i++){
    if((body_t *)list_get(f, i) == rem){
      return true;
    }
  }
  return false;
}


void update(scene_t *scene){
  list_t *bodies = scene->bodies;
  list_t *forces = scene->forces;
  int b_ind = 0;
  while(b_ind < list_size(bodies)){
    body_t *temp = (body_t *)list_get(bodies, b_ind);
    if(body_is_removed(temp)){
      int f_ind = 0;
      while(f_ind < list_size(forces)){
        if(force_is_removed((force_t *)list_get(forces, f_ind), temp)){
          force_free(list_remove(forces, f_ind));
        }
        else{
          f_ind++;
        }
      }
      body_free(list_remove(bodies, b_ind));
    }
    else{
      b_ind++;
    }
  }
}

void scene_tick(scene_t *scene, double dt) {
    list_t *forces = scene->forces;
    for(size_t i = 0; i < list_size(forces); i++){
      force_t *force = list_get(forces, i);
      force_creator_t func = force->func;
      void *aux = force->aux;
      func(aux);
    }
    update(scene);
    // if body goes off screen, get rid of assoc. forces-- imp later
    for (size_t i = 0; i < scene_bodies(scene); i++) {
        body_tick(scene_get_body(scene, i), dt);
    }
}
