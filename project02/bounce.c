#include "sdl_wrapper.h"
#include "polygon.h"
#include "vec_list.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

////////////////////////////////////////
// frame bounds, should be 4x:3y
const vector_t min = (vector_t){-100.0, -75.0};
const vector_t max = (vector_t){100.0, 75.0};
// velocity vector in units/sec
const double vel_x = 200.0;
const double vel_y = 100.0;
// rotation angle in radians/sec
const double rot_angle = 2 * (M_PI / 2);
// parameters of star, must have at least 5 sides
const size_t num_sides = 5;
const double radius_out = 15;
const double multiplier = 1;


// makes n sided star centered at (0,0) and scaled by mult; default 5 sides
vec_list_t *make_star(size_t num_pts, double mult){
  vec_list_t *star = vec_list_init(num_pts);
  // angle between consecutive points
  double angle = 2 * M_PI / num_pts;
  // inner radius based on outer, num pts
  double radius_in = (sin(M_PI * (num_sides - 4) / (2 * num_sides))) *
        (radius_out / sin(((2 * M_PI) + (num_sides * M_PI)) / (2 * num_sides)));
  // generate vectors for the outer/inner points, general case
  vector_t general_out = {radius_out, 0};
  vector_t general_in = {radius_in, 0};
  for(size_t i = 0; i < num_pts / 2; i++){
    double angle_out = (2 * i) * angle;
    // start angle for inner over bc first outer point is at 0pi
    double angle_in = ((2 * i) + 1) * angle;
    vector_t *out = malloc(sizeof(vector_t));
    assert(out != NULL);
    *out = vec_rotate(general_out, angle_out);
    vec_list_add(star, out);
    vector_t *in = malloc(sizeof(vector_t));
    assert(in != NULL);
    *in = vec_rotate(general_in, angle_in);
    vec_list_add(star, in);
  }
  // scale all vectors by mult
  for(size_t i = 0; i < vec_list_size(star); i++){
    *vec_list_get(star, i) = vec_multiply(mult, *vec_list_get(star, i));
  }
  return star;
}

/** may want to add buffer zone to prevent re-negation of velocity right
after hitting a wall due to rotation causing an adjacent point to hit the
wall consecutively **/
// reverses x and/or y velocity if any points of star start to go outside frame
void bounce(vec_list_t *star, vector_t *vel){
  // 0 if not bounced yet, else if already bounced. to prevent from bouncing
  // more than once consecutively
  int x_first = 0;
  int y_first = 0;
  for(size_t i = 0; i < vec_list_size(star); i++){
    double x = vec_list_get(star, i)->x;
    // check x, and stop loop if any one of points is over bounds
    if (x_first == 0 && (x > max.x || x < min.x)){
      // reflect velocity in x dir
      vel->x = (-1) * vel->x;
      x_first++;
    }
    // check y, and stop loop if any one of points is over bounds
    double y = vec_list_get(star, i)->y;
    if(y_first == 0 && (y > max.y || y < min.y)){
      // reflect velocity in y dir
      vel->y = (-1) * vel->y;
      y_first++;
    }
  }
}

// updates position/angle of star based on time passed (dt)
vec_list_t *compute_new_positions(vec_list_t *star, double dt, vector_t *vel){
  // change in position
  vector_t diff_pos = vec_multiply(dt, *vel);
  // change in angle
  double diff_ang = rot_angle * dt;
  vector_t center = polygon_centroid(star);
  polygon_translate(star, diff_pos);
  polygon_rotate(star, diff_ang, center);
  // check if needs to bounce
  bounce(star, vel);
  return star;
}

int main(){
  sdl_init(min, max);
  vector_t *vel = malloc(sizeof(vector_t));
  assert(vel != NULL);
  vel->x = vel_x;
  vel->y = vel_y;
  vec_list_t *star = make_star(2 * num_sides, multiplier);
  while (!sdl_is_done()){
    double dt = time_since_last_tick();
    star = compute_new_positions(star, dt, vel);
    sdl_clear();
    sdl_draw_polygon(star, 0, 1, 0);
    sdl_show();
  }
  free(vel);
  vec_list_free(star);
}
