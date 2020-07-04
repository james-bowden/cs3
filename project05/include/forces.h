#ifndef __FORCES_H__
#define __FORCES_H__

#include "scene.h"
#include "list.h"
#include "vector.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/**
* Makes aux struct, including a list of bodies and a list of constants.
*/
typedef struct aux {
    list_t *bodies;
    double constant;
} aux_t;

/**
* Initializes aux_t struct with list of bodies and list of constants.
*/
aux_t *aux_init(list_t *bodies, double constant);

/**
* Free list of bodies and constants and aux
*/
void aux_free(void *ans);

/**
* Frees nothing. wow.
*/
void free_nothing(void *thing);

// Creates the force_creator_t for gravitational force
void gravity(void *aux);

// Creates the force_creator_t for spring forces
void spring(void *aux);

// Creates the force_creator_t for drag forces
void drag(void *aux);

/**
 * Adds a force creator to a scene that applies gravity between two bodies.
 * The force creator will be called each tick
 * to compute the Newtonian gravitational force between the bodies.
 * See
 * https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation#Vector_form.
 * The force should not be applied when the bodies are very close,
 * because its magnitude blows up as the distance between the bodies goes to 0.
 *
 * @param scene the scene containing the bodies
 * @param G the gravitational proportionality constant
 * @param body1 the first body
 * @param body2 the second body
 */
void create_newtonian_gravity(scene_t *scene, double G, body_t *body1, body_t *body2);

/**
 * Adds a force creator to a scene that acts like a spring between two bodies.
 * The force creator will be called each tick
 * to compute the Hooke's-Law spring force between the bodies.
 * See https://en.wikipedia.org/wiki/Hooke%27s_law.
 *
 * @param scene the scene containing the bodies
 * @param k the Hooke's constant for the spring
 * @param body1 the first body
 * @param body2 the second body
 */
void create_spring(scene_t *scene, double k, body_t *body1, body_t *body2);

/**
 * Adds a force creator to a scene that applies a drag force on a body.
 * The force creator will be called each tick
 * to compute the drag force on the body proportional to its velocity.
 * The force points opposite the body's velocity.
 *
 * @param scene the scene containing the bodies
 * @param gamma the proportionality constant between force and velocity
 *   (higher gamma means more drag)
 * @param body the body to slow down
 */

void create_drag(scene_t *scene, double gamma, body_t *body);

/**
 * Adds a force creator to a scene that destroys two bodies when they collide.
 * The bodies should be destroyed by calling body_remove().
 *
 * @param scene the scene containing the bodies
 * @param body1 the first body
 * @param body2 the second body
 */
void create_destructive_collision(scene_t *scene, body_t *body1, body_t *body2);

#endif // #ifndef __FORCES_H__
