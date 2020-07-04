# include "vector.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

/**
 * A structure to represent 2D vectors.
 * Note that this line declares a "struct vector" type
 * and aliases it to "vector_t".
 */
typedef struct vector {
    double x;
    double y;
} vector_t;


/** @brief Allocates memory for and initializes a vector
 *         to have x-component x and y-component y
 *
 *  @param x the x-component of the new vector
 *  @param y the y-component of the new vector
 *  @return a new vector that represents <x, y>
 */
vector_t *vec_init(double x, double y){
  vector_t *v = malloc(sizeof(vector_t));
  v->x = x;
  v->y = y;
  return v;
}

/** @brief Frees the memory associated with vec
 *
 *  @param vec the vector to free
 *  @return Void.
 */
void vec_free(vector_t *vec){
  free(vec);
}

/** @brief Returns a new vector that represents the sum of v1 and v2
 *
 *  @param v1 a vector
 *  @param v2 a vector
 *  @return a vector that represents the sum of v1 and v2
 */
vector_t *vec_add(vector_t *v1, vector_t *v2){
  double x = v1->x + v2->x;
  double y = v1->y + v2->y;
  vector_t *v = vec_init(x, y);
  return v;
}
