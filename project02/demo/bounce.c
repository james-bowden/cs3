#include "sdl_wrapper.h"
#include "polygon.h"
#include "list.h"
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
const double rot_angle = 4 * (M_PI / 2);
// parameters of star, must have at least 5 sides
const size_t num_sides = 5;
const double radius_out = 10;
const double multiplier = 1;

// makes n sided star centered at start coordinate and scaled by mult with
// initial velocity and color; min 5 sides
polygon_t *make_star(size_t num_pts, double mult, vector_t start, vector_t *vel,
  float r, float g, float b){
    assert(num_pts >= 5);
    polygon_t *shape = polygon_init(num_pts);
    list_t *pts = get_points(shape);
    // account for internal points
    num_pts *= 2;
    double angle = 2 * M_PI / num_pts;
    double radius_in = radius_out / 2.0;
    // generate vectors for the outer/inner points, general case
    vector_t general_out = {radius_out, 0};
    vector_t general_in = {radius_in, 0};
    for(size_t i = 0; i < num_pts / 2; i++){
      double angle_out = (2 * i) * angle;
      double angle_in = ((2 * i) + 1) * angle;
      vector_t *out = malloc(sizeof(vector_t));
      assert(out != NULL);
      *out = vec_rotate(general_out, angle_out);
      list_add(pts, out);
      vector_t *in = malloc(sizeof(vector_t));
      assert(in != NULL);
      *in = vec_rotate(general_in, angle_in);
      list_add(pts, in);
    }
    set_velocity(shape, vel);
    set_colors(shape, r, g, b);
    polygon_scale(mult, shape);
    polygon_translate(shape, start);
    return shape;
}

// reverses x and/or y velocity if any points of shape start to go outside frame
// bounce_x and bounce_y should be set to 1 for bounce (default) and 0 for no
// adds a buffer zone so that star doesn't immediately reverse due to rotation
void bounce(polygon_t *polygon, int bounce_x, int bounce_y){
  list_t *shape = get_points(polygon);
  vector_t center = polygon_centroid(polygon);
  vector_t *vel = get_velocity(polygon);
  // 0 if not bounced yet, else if already bounced. to prevent from bouncing
  // more than once consecutively
  int x_first = 0;
  int y_first = 0;
  vector_t middle = vec_multiply(.5, vec_add(min, max));
  for(size_t i = 0; i < list_size(shape); i++){
    // second set of conditions serve as buffer to prevent rolling
    if(bounce_x == 1 && ((center.x < middle.x && vel->x < 0) ||
    (center.x > middle.x && vel->x > 0))){
      // check x, and stop loop if any one of points is over bounds
      double x = ((vector_t *)list_get(shape, i))->x;
      if(x_first == 0 && (x > max.x || x < min.x)){
        // reflect velocity in y dir
        vel->x = (-1) * vel->x;
        x_first++;
      }
    }
    if(bounce_y == 1 && ((center.y < middle.y && vel->y < 0) ||
    (center.y > middle.y && vel->y > 0))){
      // check y, and stop loop if any one of points is over bounds
      double y = ((vector_t *)list_get(shape, i))->y;
      if(y_first == 0 && (y > max.y || y < min.y)){
        // reflect velocity in y dir
        vel->y = (-1) * vel->y;
        y_first++;
      }
    }
  }
}

// updates position/angle of shape based on time passed (dt)
// x, y are 1 if bounce in that direction and 0 if not
polygon_t *compute_new_positions(polygon_t *shape, double dt, int x, int y){
  // change in velocity
  vector_t *velocity = get_velocity(shape);
  // change in position
  vector_t diff_pos = vec_multiply(dt, *velocity);
  // change in angle
  double diff_ang = rot_angle * dt;
  vector_t center = polygon_centroid(shape);
  polygon_translate(shape, diff_pos);
  polygon_rotate(shape, diff_ang, center);
  // check if needs to bounce
  bounce(shape, x, y);
  return shape;
}

int main(){
  sdl_init(min, max);
  vector_t start = (vector_t){0,0};
  vector_t *vel = malloc(sizeof(vector_t));
  assert(vel != NULL);
  vel->x = vel_x;
  vel->y = vel_y;
  float r = (rand() % (255 - 0 + 1));
  float g = (rand() % (255 - 0 + 1));
  float b = (rand() % (255 - 0 + 1));
  polygon_t *p = make_star(num_sides, multiplier, start, vel, r, g, b);
  while (!sdl_is_done()){
    double dt = time_since_last_tick();
    sdl_clear();
    // 1,1 bc we want to bounce off of all walls
    p = compute_new_positions(p, dt, 1, 1);
    sdl_draw_polygon(p);
    sdl_show();
  }
  free(vel);
  polygon_free(p);
}
