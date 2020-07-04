// n bodies, operating under spring and drag forces only

// yay pacman
#include "sdl_wrapper.h"
#include "body.h"
#include "list.h"
#include "vector.h"
#include "scene.h"
#include "forces.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <assert.h>

////////////////////////////////////////
// frame bounds, should be 4x:3y
const vector_t MIN = (vector_t){-100.0, -75.0};
const vector_t MAX = (vector_t){100.0, 75.0};
const double RADIUS = 3;
const int SIDES = 20;
const double MASS = 5;
const double K = 1;
const double GAMMA = 2;
const int GAMUT = 256;
// num bodies
const int N = 30;
const double BIG_NUM = 1e10;


// body_t *make_pacman(){
//   vector_t *center = malloc(sizeof(vector_t));
//   assert(center != NULL);
//   *center = vec_add(MIN, MAX);
//   list_t *points = list_init(PAC_SIDES + 1, vec_free);
//   vector_t general_pt = (vector_t){RADIUS_PAC, 0};
//   double rot = ((2 * M_PI) - MOUTH_ANGLE) / PAC_SIDES;
//   list_add(points, center);
//   double angle = MOUTH_ANGLE / 2;
//   for(size_t i = 0; i < PAC_SIDES; i++){
//     vector_t *temp = malloc(sizeof(vector_t));
//     assert(temp != NULL);
//     *temp = vec_rotate(general_pt, angle + (rot * i));
//     list_add(points, temp);
//   }
//   body_t *pacman = body_init(points, MASS, YELLOW);
//   return pacman;
// }

body_t *make_circle(vector_t start, rgb_color_t color, double mass){
  list_t *points = list_init(SIDES, vec_free);
  vector_t general_pt = (vector_t){RADIUS, 0};
  double rot = (2 * M_PI) / SIDES;
  for(size_t i = 0; i < SIDES; i++){
    vector_t *temp = malloc(sizeof(vector_t));
    assert(temp != NULL);
    *temp = vec_rotate(general_pt, rot * i);
    list_add(points, temp);
  }
  body_t *circle = body_init(points, mass, color);
  body_translate(circle, start);
  return circle;
}

rgb_color_t make_rand_color(){
  float r = 1;
  float g = 1;
  float b = 1;
  // prevent totally white shape
  while (r == 1 && g == 1 && b == 1){
    r = (float)(rand() % GAMUT);
    g = (float)(rand() % GAMUT);
    b = (float)(rand() % GAMUT);
  }
  rgb_color_t ans = (rgb_color_t){r, g, b};
  return ans;
}

void add_forces(scene_t *scene){
  for(size_t i = 0; i < scene_bodies(scene); i++){
    body_t *b1 = scene_get_body(scene, i);
    for(size_t j = i+1; j < scene_bodies(scene); j++){
      body_t *b2 = scene_get_body(scene, j);
      create_spring(scene, K, b1, b2);
    }
    create_drag(scene, GAMMA, b1);
  }
}

int main(){
  sdl_init(MIN, MAX);
  scene_t *scene = scene_init();
  vector_t left = {MIN.x, (MIN.y+MAX.y)/2};
  vector_t increment = {2*RADIUS, 0};
  for(int i = 0; i < N; i++){
    vector_t shift = vec_add(left, vec_multiply(i, increment));
    body_t *circle = make_circle(shift, make_rand_color(), MASS);
    scene_add_body(scene, circle);
  }
  // make anchor
  // vector_t shift = vec_add(left, vec_multiply(N, increment));
  // scene_add_body(scene, make_circle(shift, (rgb_color_t){0, 0, 0}, INFINITY));
  add_forces(scene);
  while (!sdl_is_done(scene)){
    double dt = time_since_last_tick();
    scene_tick(scene, dt);
    sdl_render_scene(scene);
  }
  scene_free(scene);
}
