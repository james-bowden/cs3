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
const double vel_x = 75.0;
const double vel_y = -50.0;
// rotation angle in radians/sec
const double rot_angle = 3 * (M_PI / 2);
// acceleration in units/sec^2
const vector_t accel_y = (vector_t){0, -1000.0};
// elasticity as a decimal multiplier
const double elasticity = .8;
// base parameters of shape
const double radius_out = 10;
const double mult = 1;


// makes n sided polygon centered at start coordinate and scaled by mult with
// initial velocity and color; min 3 sides
polygon_t *make_ngon(size_t num_pts, double mult, vector_t start, vector_t *vel,
  float r, float g, float b){
    assert(num_pts >= 3);
    polygon_t *shape = polygon_init(num_pts);
    list_t *pts = get_points(shape);
    double angle = 2 * M_PI / num_pts;
    // generate vectors for general case points
    vector_t general_pt = (vector_t){radius_out, 0};
    for(size_t i = 0; i < num_pts; i++){
      double angle_ver = (i) * angle;
      vector_t *vertices = malloc(sizeof(vector_t));
      assert(vertices != NULL);
      *vertices = vec_rotate(general_pt, angle_ver);
      list_add(pts, vertices);
    }
    set_velocity(shape, vel);
    set_colors(shape, r, g, b);
    polygon_scale(mult, shape);
    polygon_translate(shape, start);
    return shape;
}

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
// adds a buffer zone so that shape doesn't immediately reverse due to rotation
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
        // reflect velocity in y dir, using elasticity as multiplier
        vel->y = (-1) * elasticity * vel->y;
        y_first++;
      }
    }
  }
}

// updates position/angle of shape based on time passed (dt)
// x and y can each be 1 for yes bounce and 0 for no bounce
polygon_t *compute_new_positions(polygon_t *shape, double dt, int x, int y){
  // change in velocity
  vector_t *velocity = get_velocity(shape);
  *velocity = vec_add(*velocity, vec_multiply(dt, accel_y));
  // change in position
  vector_t diff_pos = vec_multiply(dt, *velocity);
  // change in angle
  double diff_ang = rot_angle * dt;
  vector_t center = polygon_centroid(shape);
  polygon_translate(shape, diff_pos);
  polygon_rotate(shape, diff_ang, center);
  // check if needs to bounce; (0,1) because no bounce in x dir.
  bounce(shape, x, y);
  return shape;
}

// return 0 if shape in bounds, 1 if all points out of bounds
int out_of_bounds(polygon_t *shape){
  list_t *points = get_points(shape);
  for(int i = 0; i < list_size(points); i++){
    if(((vector_t *)list_get(points, i))->x < max.x) {
      return 0;
    }
  }
  return 1;
}

int main(){
  sdl_init(min, max);
  vector_t start = (vector_t){min.x + radius_out, max.y - radius_out};
  vector_t *vel = malloc(sizeof(vector_t));
  assert(vel != NULL);
  vel->x = vel_x;
  vel->y = vel_y;
  list_t *shape_list = list_init(10);
  int ind = 0;
  while (!sdl_is_done()){
    // 1 for ngon, 0 for star
    int shape = rand() % 2;
    float r = 1;
    float g = 1;
    float b = 1;
    // prevent totally white shape
    while (r == 1 && g == 1 && b == 1){
      r = (rand() % (255 - 0 + 1));
      g = (rand() % (255 - 0 + 1));
      b = (rand() % (255 - 0 + 1));
    }
    size_t num_pts = (rand() % (10 - 3 + 1)) + 3;
    double dt = time_since_last_tick();
    // enter new shape when last shape reaches peak of bounce, or if first
    if(ind == 0){
      polygon_t *p = make_ngon(num_pts, mult, start, vel, r, g, b);
      list_add(shape_list, p);
      ind++;
    }
    else{
      polygon_t *last = (polygon_t *)list_get(shape_list, ind - 1);
      // range for vel in case steps skip over vel = 0
      if (get_velocity(last)->y < 5 && get_velocity(last)->y > -5){
        polygon_t *p;
        if((shape == 1 && num_pts < 9) || num_pts < 5){
          p = make_ngon(num_pts, mult, start, vel, r, g, b);
        }
        else{
          p = make_star(num_pts, mult, start, vel, r, g, b);
        }
        list_add(shape_list, p);
        ind++;
      }
    }
    //
    sdl_clear();
    for(int i = 0; i < ind; i++){
      polygon_t *p = (polygon_t *)list_get(shape_list, i);
      if(out_of_bounds(p) == 0){
        // 0, 1 so bounces for y only
        p = compute_new_positions(p, dt, 0, 1);
        sdl_draw_polygon(p);
      }
    }
    sdl_show();
  }
  free(vel);
  list_free(shape_list, polygon_free);
}
