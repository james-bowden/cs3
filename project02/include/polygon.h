#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "list.h"
#include "vector.h"

typedef struct polygon polygon_t;

/**
* initialize new polygon object.
*
* @param size_t num_pts
**/
polygon_t *polygon_init(size_t num_pts);

/**
 * Free pointers of polygons. Is of type freer_t and can be passed in as arg.
 */
 /**
  * Frees a vector pointer.
  *
  * @param v, polygon_t pointer
  */
typedef void (*freer_t)(void *x);
void polygon_free(void *x);

/**
 * Set points of polygon given vec_list_t
 * @param vec_list_t of points for each polygon
 * @param polygon_t * polygon
 */
void set_points(polygon_t *polygon, list_t * points);

/**
 * Set velocity of polygon given velocity vector
 * @param vector_t value with x and y velocity
 * @param polygon_t * polygon
 */
void set_velocity(polygon_t *polygon, vector_t *velocity);

/**
 * Set color of polygon given float rgb values
 * @param float rgb values that range from 0 to 255
 * @param polygon_t * polygon
 */
void set_colors(polygon_t *polygon, float red, float green, float blue);

/**
 * Set points, velocity, and RGB color of polygon object
 * @param polygon object, list of points/vertices, velocity, and RGB
 * float values
 */
void set_all(polygon_t *polygon, list_t *points, vector_t *velocity,
  float r, float g, float b);
/**
* Return float color corresponding to red
* @param polygon_t *polygon
*/
float get_r(polygon_t *polygon);
/**
* Return float color corresponding to green
* @param polygon_t *polygon
 */
float get_g(polygon_t *polygon);
/**
 * Return float color corresponding to blue
 * @param polygon_t *polygon
 */
float get_b(polygon_t *polygon);

/**
 * Returns list of the points in polygon
 * @param polygon_t *polygon
 */
list_t *get_points(polygon_t *polygon);

/**
 * Returns the vector_t velocity of the polygon
 * @param polygon_t *polygon
 */
vector_t *get_velocity(polygon_t *polygon);

/**
 * Computes the area of a polygon.
 * See https://en.wikipedia.org/wiki/Shoelace_formula#Statement.
 *
 * @param polygon object with the list of vertices that make up the polygon,
 * listed in a counterclockwise direction. There is an edge between
 * each pair of consecutive vertices, plus one between the first and last.
 * @return the area of the polygon
 */
double polygon_area(polygon_t *polygon);

/**
 * Computes the center of mass of a polygon.
 * See https://en.wikipedia.org/wiki/Centroid#Of_a_polygon.
 *
 * @param polygon object with the list of vertices that make up the polygon,
 * listed in a counterclockwise direction. There is an edge between
 * each pair of consecutive vertices, plus one between the first and last.
 * @return the centroid of the polygon
 */
vector_t polygon_centroid(polygon_t *polygon);

/**
 * Translates all vertices in a polygon by a given vector.
 * Note: mutates the original polygon.
 *
 * @param polygon object with the list of vertices that make up the polygon
 * @param translation the vector to add to each vertex's position
 */
void polygon_translate(polygon_t *polygon, vector_t translation);

/**
 * Rotates vertices in a polygon by a given angle about a given point.
 * Note: mutates the original polygon.
 *
 * @param polygon object with the list of vertices that make up the polygon
 * @param angle the angle to rotate the polygon, in radians.
 * A positive angle means counterclockwise.
 * @param point the point to rotate around
 */
void polygon_rotate(polygon_t *polygon, double angle, vector_t point);

/**
 * Scales vertices in a polygon by a given multiplier.
 * Note: mutates the original polygon.
 *
 * @param polygon object with the list of vertices that make up the polygon
 * @param mult the double to scale by
 */
void polygon_scale(double mult, polygon_t *polygon);

#endif // #ifndef __POLYGON_H__
