//# include "vector.h"
# include <stdio.h>
# include <stdlib.h>
# include "vector.c"

// tester for vector.c

int main(){
  printf("< x = 1, y = 1\n"); // input
  vector_t *v = vec_init(1, 1);
  printf("x = %f, y = %f\n", v->x, v->y); // output
  //
  printf("Test double: < x = 4.8, y = 10.93\n"); // input
  vector_t *w = vec_init(4.8, 10.93);
  printf("x = %f, y = %f\n", w->x, w->y); // output
  //
  printf("Test vector addition: < x = 5.8, y = 11.93\n"); // input
  vector_t *x = vec_add(v, w);
  printf("x = %f, y = %f\n", x->x, x->y); // output
  //
  printf("< x = -3, y = -7.7\n"); // input
  vector_t *y = vec_init(-3, -7.7);
  printf("x = %f, y = %f\n", y->x, y->y); // output
  //
  printf("< x = 8.2, y = -4.1\n"); // input
  vector_t *z = vec_init(8.2, -4.1);
  printf("x = %f, y = %f\n", z->x, z->y); // output
  //
  printf("Test vector addition (neg+pos): < x = 1.8, y = 3.23\n"); // input
  vector_t *a = vec_add(y, w);
  printf("x = %f, y = %f\n", a->x, a->y); // output
  //
  printf("Test vector addition (neg+neg): < x = 5.2, y = -11.8\n"); // input
  vector_t *b = vec_add(y, z);
  printf("x = %f, y = %f\n", b->x, b->y); // output
  //

  vec_free(v);
  vec_free(w);
  vec_free(x);
  vec_free(y);
  vec_free(z);
  vec_free(a);
  vec_free(b);

  return 0;
}
