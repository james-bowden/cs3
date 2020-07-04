#include "scene.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


const size_t num_bodies = 25;


typedef struct scene{
  list_t *bodies;
} scene_t;

scene_t *scene_init(void){
  scene_t *scene = malloc(sizeof(scene_t));
  assert(scene != NULL);
  scene->bodies = list_init(num_bodies, body_free);
  return scene;
}

void scene_free(scene_t *scene){
  list_free(scene->bodies);
  free(scene);
}

size_t scene_bodies(scene_t *scene){
  return list_size(scene->bodies);
}

body_t *scene_get_body(scene_t *scene, size_t index){
  return (body_t *) list_get(scene->bodies, index);
}

list_t *scene_get_all_bodies(scene_t *scene){
  return scene->bodies;
}

void scene_add_body(scene_t *scene, body_t *body){
  list_add(scene->bodies, body);
}

void scene_remove_body(scene_t *scene, size_t index){
  list_remove(scene->bodies, index);
}

void scene_tick(scene_t *scene, double dt){
  for(size_t i = 0; i < scene_bodies(scene); i++){
    body_tick(scene_get_body(scene, i), dt);
  }
}
