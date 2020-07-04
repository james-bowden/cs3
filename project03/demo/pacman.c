// yay pacman
#include "sdl_wrapper.h"
#include "body.h"
#include "list.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

////////////////////////////////////////
// frame bounds, should be 4x:3y
const vector_t MIN = (vector_t){-100.0, -75.0};
const vector_t MAX = (vector_t){100.0, 75.0};
// acceleration in units/sec^2 (used for both x and y direction)
const double ACCEL = .05;
// base parameters of pacman, dots. mass = 0 bc no gravity here
const double RADIUS_PAC = 7;
const double RADIUS_PELLET = 1;
const rgb_color_t YELLOW = {255, 211, 0};
const double MOUTH_ANGLE = M_PI / 3;
const double INIT_VEL = 3;
const int PAC_SIDES = 20;
const int PELLET_SIDES = 20;
const double MASS = 0;
const double MULT = 1;
// Number of pellets in scene at any one time
const int MAX_PELLETS = 40;


body_t *make_pacman(){
  vector_t *center = malloc(sizeof(vector_t));
  assert(center != NULL);
  *center = vec_add(MIN, MAX);
  list_t *points = list_init(PAC_SIDES + 1, vec_free);
  vector_t general_pt = (vector_t){RADIUS_PAC, 0};
  double rot = ((2 * M_PI) - MOUTH_ANGLE) / PAC_SIDES;
  list_add(points, center);
  double angle = MOUTH_ANGLE / 2;
  for(size_t i = 0; i < PAC_SIDES; i++){
    vector_t *temp = malloc(sizeof(vector_t));
    assert(temp != NULL);
    *temp = vec_rotate(general_pt, angle + (rot * i));
    list_add(points, temp);
  }
  body_t *pacman = body_init(points, MASS, YELLOW);
  return pacman;
}

// put pacman on the other side if he goes oob
void wrap(scene_t *scene){
  body_t *pacman = scene_get_body(scene, 0);
  list_t *points = body_get_shape(pacman);
  vector_t *center = (vector_t *)list_get(points, 0);
  if((center->x + RADIUS_PAC) < MIN.x){
    body_translate(pacman, (vector_t){MAX.x - MIN.x, 0});
  }
  if((center->x - RADIUS_PAC) > MAX.x){
    body_translate(pacman, (vector_t){MIN.x - MAX.x, 0});
  }
  if((center->y + RADIUS_PAC) < MIN.y){
    body_translate(pacman, (vector_t){0, MAX.y - MIN.y});
  }
  if((center->y - RADIUS_PAC) > MAX.y){
    body_translate(pacman, (vector_t){0, MIN.y - MAX.y});
  }
  list_free(points);
}

// makes a pellet centered around center and adds to scene
void make_pellet(scene_t *scene, vector_t center){
  list_t *points = list_init(PELLET_SIDES, vec_free);
  vector_t buffer_right = {0, RADIUS_PELLET};
  for(int i = 0; i < PELLET_SIDES; ++i){
    vector_t *next = malloc(sizeof(vector_t));
    assert(next != NULL);
    *next = vec_rotate(buffer_right, 2 * M_PI * i / PELLET_SIDES);
    list_add(points, next);
  }
  body_t *ans = body_init(points, MASS, YELLOW);
  body_translate(ans, center);
  scene_add_body(scene, ans);
}

// checks all pellets; if any touching pacman removes them and spawns new
void remove_pellets(scene_t *scene){
  list_t *points = body_get_shape(scene_get_body(scene, 0));
  vector_t *center = (vector_t *)list_get(points, 0);
  size_t i = 1;
  while(i < scene_bodies(scene)){
    body_t *pellet = scene_get_body(scene, i);
    vector_t center_pellet = body_get_centroid(pellet);
    double pellet_x = center_pellet.x;
    double pellet_y = center_pellet.y;
    if(pellet_x <= center->x + RADIUS_PAC &&
      pellet_x >= center->x - RADIUS_PAC &&
      pellet_y <= center->y + RADIUS_PAC &&
      pellet_y >= center->y - RADIUS_PAC){
        scene_remove_body(scene, i);
        vector_t cen = (vector_t){(int)rand() % (int)(MAX.x + 1 - MIN.x) + MIN.x,
                          (int)rand() % (int)(MAX.y + 1 - MIN.y) + MIN.y};
        make_pellet(scene, cen);
      }
    else{
      i++;
    }
  }
  list_free(points);
}

// move pacman in accordance with key presses
void move_pacman(scene_t *scene, char key, key_event_type_t type, double dt){
  double dv = dt * ACCEL;
  body_t *pacman = scene_get_body(scene, 0);
  if (type == KEY_PRESSED && type != KEY_RELEASED) {
    switch(key) {
      case UP_ARROW:
        body_set_rotation(pacman, M_PI / 2.0);
        body_set_velocity(pacman, (vector_t){0, INIT_VEL});
        body_set_velocity(pacman, (vector_t){0, body_get_velocity(pacman).y + dv});
        break;
      case DOWN_ARROW:
        body_set_rotation(pacman, -1.0 * M_PI / 2.0);
        body_set_velocity(pacman, (vector_t){0, -INIT_VEL});
        body_set_velocity(pacman, (vector_t){0, body_get_velocity(pacman).y - dv});
        break;
      case LEFT_ARROW:
        body_set_rotation(pacman, M_PI);
        body_set_velocity(pacman, (vector_t){-INIT_VEL, 0});
        body_set_velocity(pacman, (vector_t){body_get_velocity(pacman).x - dv, 0});
        break;
      case RIGHT_ARROW:
        body_set_rotation(pacman, 0.0);
        body_set_velocity(pacman, (vector_t){INIT_VEL, 0});
        body_set_velocity(pacman, (vector_t){body_get_velocity(pacman).x + dv, 0});
        break;
    }
  }
  scene_tick(scene, dt);
  wrap(scene);
  remove_pellets(scene);
}

int main(){
  sdl_init(MIN, MAX);
  scene_t *scene = scene_init();
  body_t *pacman = make_pacman();
  scene_add_body(scene, pacman);
  for(int i = 0; i < MAX_PELLETS; i++){
    vector_t center = (vector_t){(int)rand() % (int)(MAX.x + 1 - MIN.x) + MIN.x,
                      (int)rand() % (int)(MAX.y + 1 - MIN.y) + MIN.y};
    make_pellet(scene, center);
  }
  sdl_on_key(move_pacman);
  while (!sdl_is_done(scene)){
    double dt = time_since_last_tick();
    sdl_render_scene(scene);
  }
  scene_free(scene);
}
