#include "sdl_wrapper.h"
#include "polygon.h"
#include "vec_list.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// parameters as global constants
////////////////////////////////////////
// frame bounds, should be 2x:y
double min_x = -100.0;
double min_y = -75.0;
double max_x = 100.0;
double max_y = 75.0;
// velocity vector in units/sec
double vel_x = 200.0;
double vel_y = 100.0;
// rotation angle in radians/sec
const double angle = 2 * (M_PI / 2);


// makes 5 sided star centered at (0,0) and scaled by mult
vec_list_t *make_star(double mult){
  // number of points for 5 sided star
  size_t num_pts = 10;
  vec_list_t *star = vec_list_init(num_pts);
  // angle between consecutive points
  double angle = M_PI / 5;
  // radii for outer/inner points
  double radius_out = 10;
  double radius_in = sin(M_PI / 10) * radius_out / sin(7 * M_PI / 10);
  // generate vectors for the 5 outer points, general case
  vector_t general_out = {radius_out, 0};
  // generate vectors for 5 inner points, general case
  vector_t general_in = {radius_in, 0};
  for(size_t i = 0; i < num_pts / 2; i++){
    double angle_out = (2 * i) * angle;
    // start angle for inner over bc first outer point is at 0pi
    double angle_in = ((2 * i) + 1) * angle;
    // even indecies bc alternates btwn inner/outer points
    vector_t *out = malloc(sizeof(vector_t));
    *out = vec_rotate(general_out, angle_out);
    vec_list_add(star, out);
    // odd indecies bc alternates btwn inner/outer points
    vector_t *in = malloc(sizeof(vector_t));
    *in = vec_rotate(general_in, angle_in);
    vec_list_add(star, in);
  }
  // scale all vectors by mult
  for(size_t i = 0; i < vec_list_size(star); i++){
    *vec_list_get(star, i) = vec_multiply(mult, *vec_list_get(star, i));
  }
  return star;
}

/** may need to add buffer zone to prevent re-negation of velocity right
after hitting a wall due to rotation causing an adjacent point to hit the
wall consecutively **/
// reverses x and/or y velocity if any points of star start to go outside frame
void bounce(vec_list_t *star, vector_t *vel){
  // check x, and stop loop if any one of points is over bounds
  for(size_t i = 0; i < vec_list_size(star); i++){
    double x = vec_list_get(star, i)->x;
    if (x > max_x || x < min_x){
      // reflect velocity in x dir
      vel->x = (-1) * vel->x;
      break;
    }
  }
  // check y, and stop loop if any one of points is over bounds
  for(size_t i = 0; i < vec_list_size(star); i++){
    double y = vec_list_get(star, i)->y;
    if(y > max_y || y < min_y){
      // reflect velocity in y dir
      vel->y = (-1) * vel->y;
      break;
    }
  }
}

// updates position/angle of star based on time passed (dt)
vec_list_t *compute_new_positions(vec_list_t *star, double dt, vector_t *vel){
  // change in position
  vector_t diff_pos = vec_multiply(dt, *vel);
  // change in angle
  double diff_ang = angle * dt;
  // current center of star
  vector_t center = polygon_centroid(star);
  // if does not need to bounce, continue on
  polygon_translate(star, diff_pos);
  polygon_rotate(star, diff_ang, center);
  // check if needs to bounce by seeing if any of the points out of frame
  bounce(star, vel);
  return star;
}

int main(){
  // min and max coordinates of window
  vector_t *min = malloc(sizeof(vector_t));
  vector_t *max = malloc(sizeof(vector_t));
  min->x = min_x;
  min->y = min_y;
  max->x = max_x;
  max->y = max_y;
  sdl_init(*min, *max);
  // init velocity vector
  vector_t *vel = malloc(sizeof(vector_t));
  vel->x = vel_x;
  vel->y = vel_y;
  vec_list_t *star = make_star(1);
  while (!sdl_is_done()){
    double dt = time_since_last_tick();
    star = compute_new_positions(star, dt, vel);
    sdl_clear();
    sdl_draw_polygon(star, 0, 1, 0);
    sdl_show();
  }
  free(vel);
  free(min);
  free(max);
  vec_list_free(star);
}
