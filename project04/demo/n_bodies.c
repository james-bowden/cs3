// n bodies, operating under gravity toward each other. uses gravity force only

// yay NOT pacman
#include "sdl_wrapper.h"
#include "list.h"
#include "vector.h"
#include "scene.h"
#include "forces.h"
#include "body.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <assert.h>

////////////////////////////////////////
// frame bounds, should be 4x:3y
const vector_t MIN = (vector_t){-100.0, -75.0};
const vector_t MAX = (vector_t){100.0, 75.0};
const double RADIUS = 5;
const double INIT_VEL = 0;
const int SIDES = 5;
const double MASS = 10;
// num bodies
const double G = 1000;
const int N = 30;
// The scale for the inner radius, angle
const double RADIUS_SCALE = 2.0;
const double ANGLE_SCALE = RADIUS_SCALE;
const int GAMUT = 256;


// body_t *make_pacman(){
//   vector_t *center = malloc(sizeof(vector_t));
//   assert(center != NULL);
//   *center = start;
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

// makes 5 sided star centered at start coordinate and scaled by mult with
// initial velocity and color; min 5 sides
body_t *make_star(vector_t start, rgb_color_t color){
  vector_t velocity = (vector_t){INIT_VEL, INIT_VEL};
  list_t *pts = list_init(SIDES, vec_free);
  // account for internal points
  double angle = M_PI / SIDES;
  double radius_in = RADIUS / RADIUS_SCALE;
  // generate vectors for the outer/inner points, general case
  vector_t general_out = {RADIUS, 0};
  vector_t general_in = {radius_in, 0};
  for(size_t i = 0; i < SIDES; i++){
    double angle_out = (ANGLE_SCALE * i) * angle;
    double angle_in = ((ANGLE_SCALE * i) + 1) * angle;
    vector_t *out = malloc(sizeof(vector_t));
    assert(out != NULL);
    *out = vec_rotate(general_out, angle_out);
    list_add(pts, out);
    vector_t *in = malloc(sizeof(vector_t));
    assert(in != NULL);
    *in = vec_rotate(general_in, angle_in);
    list_add(pts, in);
  }
  body_t *body = body_init(pts, MASS, color);
  body_translate(body, start);
  body_set_velocity(body, velocity);
  return body;
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

void add_gravity(scene_t *scene){
  for(size_t i = 0; i < scene_bodies(scene); i++){
    for(size_t j = i+1; j < scene_bodies(scene); j++){
      body_t *b1 = scene_get_body(scene, i);
      body_t *b2 = scene_get_body(scene, j);
      create_newtonian_gravity(scene, G, b1, b2);
    }
  }
}

int main(){
  sdl_init(MIN, MAX);
  scene_t *scene = scene_init();
  for(int i = 0; i < N; i++){
    // not accounting for rad
    double x = (int)rand() % (int)(MAX.x + 1 - MIN.x) + MIN.x;
    double y = (int)rand() % (int)(MAX.y + 1 - MIN.y) + MIN.y;
    vector_t start = (vector_t){x, y};
    body_t *star = make_star(start, make_rand_color());
    scene_add_body(scene, star);
  }
  add_gravity(scene);
  while (!sdl_is_done(scene)){
    double dt = time_since_last_tick();
    scene_tick(scene, dt);
    sdl_render_scene(scene);
  }
  scene_free(scene);
}
