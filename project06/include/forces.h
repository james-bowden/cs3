#ifndef __FORCES_H__
#define __FORCES_H__

#include "scene.h"
#include "list.h"
#include "vector.h"
#include "collision.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/**
 * A function called when a collision occurs.
 * @param body1 the first body passed to create_collision()
 * @param body2 the second body passed to create_collision()
 * @param axis a unit vector pointing from body1 towards body2
 *   that defines the direction the two bodies are colliding in
 * @param aux the auxiliary value passed to create_collision()
 */
typedef void (*collision_handler_t)
    (body_t *body1, body_t *body2, vector_t axis, void *aux);

/**
* Makes aux struct, including a list of bodies and a list of constants.
*/
typedef struct aux {
    void *aux;
    list_t *bodies;
    double constant;
    collision_handler_t handler;
    bool collided;
    free_func_t freer;
} aux_t;

// for use retrieving addtl aux inside when making own collision
void *aux_get_aux(aux_t *aux);

/**
* Initializes aux_t struct with list of bodies and list of constants.
*/
aux_t *aux_init(list_t *bodies, double constant);

// set the func; not incl. in init bc don't want to do for the actual forces
void aux_set_handler(aux_t *aux, collision_handler_t func);

// ""
void aux_set_aux(aux_t *aux, void *info);

// freer for void * aux
void aux_set_freer(aux_t *aux, free_func_t freer);

// to free aux_t inside aux_t since bodies are shared.
void aux_ception_free(void *aux);

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
 * Adds a force creator to a scene that calls a given collision handler
 * function each time two bodies collide.
 * This generalizes create_destructive_collision() from last week,
 * allowing different things to happen on a collision.
 * The handler is passed the bodies, the collision axis, and an auxiliary value.
 * It should only be called once while the bodies are still colliding.
 *
 * @param scene the scene containing the bodies
 * @param body1 the first body
 * @param body2 the second body
 * @param handler a function to call whenever the bodies collide
 * @param aux an auxiliary value to pass to the handler
 * @param freer if non-NULL, a function to call in order to free aux
 */
void create_collision(
    scene_t *scene,
    body_t *body1,
    body_t *body2,
    collision_handler_t handler,
    void *aux,
    free_func_t freer
);

/**
 * Adds a force creator to a scene that destroys two bodies when they collide.
 * The bodies should be destroyed by calling body_remove().
 * This should be represented as an on-collision callback
 * registered with create_collision().
 *
 * @param scene the scene containing the bodies
 * @param body1 the first body
 * @param body2 the second body
 */
void create_destructive_collision(scene_t *scene, body_t *body1, body_t *body2);


void impulse(body_t *body1, body_t *body2, vector_t axis, void *aux);

/**
 * Adds a force creator to a scene that applies impulses
 * to resolve collisions between two bodies in the scene.
 * This should be represented as an on-collision callback
 * registered with create_collision().
 *
 * You may remember from project01 that you should avoid applying impulses
 * multiple times while the bodies are still colliding.
 * You should also have a special case that allows either body1 or body2
 * to have mass INFINITY, as this is useful for simulating walls.
 *
 * @param scene the scene containing the bodies
 * @param elasticity the "coefficient of restitution" of the collision;
 * 0 is a perfectly inelastic collision and 1 is a perfectly elastic collision
 * @param body1 the first body
 * @param body2 the second body
 */
void create_physics_collision(
    scene_t *scene,
    double elasticity,
    body_t *body1,
    body_t *body2,
    int id
);

void destroy_brick(body_t *body1, body_t *body2, vector_t axis, void *aux);

#endif // #ifndef __FORCES_H__
