#include "polygon.h"
#include "test_util.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

// just to fill
const vector_t vel = {0, 0};

// Make square at (+/-1, +/-1)
polygon_t *make_square() {
    polygon_t *square = polygon_init(4);
    list_t *sq = get_points(square);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t) {+1, +1};
    list_add(sq, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {-1, +1};
    list_add(sq, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {-1, -1};
    list_add(sq, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {+1, -1};
    list_add(sq, v);
    return square;
}

void test_square_area_centroid() {
    polygon_t *square = make_square();
    assert(isclose(polygon_area(square), 4));
    assert(vec_isclose(polygon_centroid(square), VEC_ZERO));
    polygon_free(square);
}

void test_square_translate() {
    polygon_t *square = make_square();
    list_t *sq = get_points(square);
    polygon_translate(square, (vector_t) {2, 3});
    assert(vec_equal(*(vector_t *)list_get(sq, 0), (vector_t) {3, 4}));
    assert(vec_equal(*(vector_t *)list_get(sq, 1), (vector_t) {1, 4}));
    assert(vec_equal(*(vector_t *)list_get(sq, 2), (vector_t) {1, 2}));
    assert(vec_equal(*(vector_t *)list_get(sq, 3), (vector_t) {3, 2}));
    assert(isclose(polygon_area(square), 4));
    assert(vec_isclose(polygon_centroid(square), (vector_t) {2, 3}));
    polygon_free(square);
}

void test_square_rotate() {
    polygon_t *square = make_square();
    list_t *sq = get_points(square);
    polygon_rotate(square, 0.25 * M_PI, VEC_ZERO);
    assert(vec_isclose(*(vector_t *)list_get(sq, 0), (vector_t) {0, sqrt(2)}));
    assert(vec_isclose(*(vector_t *)list_get(sq, 1), (vector_t) {-sqrt(2), 0}));
    assert(vec_isclose(*(vector_t *)list_get(sq, 2), (vector_t) {0, -sqrt(2)}));
    assert(vec_isclose(*(vector_t *)list_get(sq, 3), (vector_t) {sqrt(2), 0}));
    assert(isclose(polygon_area(square), 4));
    assert(vec_isclose(polygon_centroid(square), VEC_ZERO));
    polygon_free(square);
}

// Make 3-4-5 triangle
polygon_t *make_triangle() {
    polygon_t *triangle = polygon_init(3);
    list_t *tri = get_points(triangle);
    vector_t *v = malloc(sizeof(*v));
    *v = VEC_ZERO;
    list_add(tri, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {4, 0};
    list_add(tri, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {4, 3};
    list_add(tri, v);
    return triangle;
}

void test_triangle_area_centroid() {
    polygon_t *triangle = make_triangle();
    assert(isclose(polygon_area(triangle), 6));
    assert(vec_isclose(polygon_centroid(triangle), (vector_t) {8.0 / 3.0, 1}));
    polygon_free(triangle);
}

void test_triangle_translate() {
    polygon_t *triangle = make_triangle();
    list_t *tri = get_points(triangle);
    polygon_translate(triangle, (vector_t) {-4, -3});
    assert(vec_equal(*(vector_t *)list_get(tri, 0), (vector_t) {-4, -3}));
    assert(vec_equal(*(vector_t *)list_get(tri, 1), (vector_t) {0,  -3}));
    assert(vec_equal(*(vector_t *)list_get(tri, 2), (vector_t) {0,  0}));
    assert(isclose(polygon_area(triangle), 6));
    assert(vec_isclose(polygon_centroid(triangle), (vector_t) {-4.0 / 3.0, -2}));
    polygon_free(triangle);
}

void test_triangle_rotate() {
    polygon_t *triangle = make_triangle();
    list_t *tri = get_points(triangle);
    // Rotate -acos(4/5) degrees around (4,3)
    polygon_rotate(triangle, -acos(4.0 / 5.0), (vector_t) {4, 3});
    assert(vec_isclose(*(vector_t *)list_get(tri, 0), (vector_t) {-1,  3}));
    assert(vec_isclose(*(vector_t *)list_get(tri, 1), (vector_t) {2.2, 0.6}));
    assert(vec_isclose(*(vector_t *)list_get(tri, 2), (vector_t) {4,   3}));
    assert(isclose(polygon_area(triangle), 6));
    assert(vec_isclose(polygon_centroid(triangle), (vector_t) {26.0 / 15.0, 2.2}));

    polygon_free(triangle);
}

#define CIRC_NPOINTS 1000000
#define CIRC_AREA (CIRC_NPOINTS * sin(2 * M_PI / CIRC_NPOINTS) / 2)

// Circle with many points (stress test)
polygon_t *make_big_circ() {
    polygon_t *circle = polygon_init(CIRC_NPOINTS);
    list_t *c = get_points(circle);
    for (size_t i = 0; i < CIRC_NPOINTS; i++) {
        double angle = 2 * M_PI * i / CIRC_NPOINTS;
        vector_t *v = malloc(sizeof(*v));
        *v = (vector_t) {cos(angle), sin(angle)};
        list_add(c, v);
    }
    return circle;
}

void test_circ_area_centroid() {
    polygon_t *circle = make_big_circ();
    assert(isclose(polygon_area(circle), CIRC_AREA));
    assert(vec_isclose(polygon_centroid(circle), VEC_ZERO));
    polygon_free(circle);
}

void test_circ_translate() {
    polygon_t *circle = make_big_circ();
    list_t *c = get_points(circle);
    polygon_translate(circle, (vector_t) {100, 200});

    for (size_t i = 0; i < CIRC_NPOINTS; i++) {
        double angle = 2 * M_PI * i / CIRC_NPOINTS;
        assert(vec_isclose(
            *(vector_t *)list_get(c, i),
            (vector_t) {100 + cos(angle), 200 + sin(angle)}
        ));
    }
    assert(isclose(polygon_area(circle), CIRC_AREA));
    assert(vec_isclose(polygon_centroid(circle), (vector_t) {100, 200}));

    polygon_free(circle);
}

void test_circ_rotate() {
    // Rotate about the origin at an unusual angle
    const double ROT_ANGLE = 0.5;

    polygon_t *circle = make_big_circ();
    list_t *c = get_points(circle);
    polygon_rotate(circle, ROT_ANGLE, VEC_ZERO);

    for (size_t i = 0; i < CIRC_NPOINTS; i++) {
        double angle = 2 * M_PI * i / CIRC_NPOINTS;
        assert(vec_isclose(
            *(vector_t *)list_get(c, i),
            (vector_t) {cos(angle + ROT_ANGLE), sin(angle + ROT_ANGLE)}
        ));
    }
    assert(isclose(polygon_area(circle), CIRC_AREA));
    assert(vec_isclose(polygon_centroid(circle), VEC_ZERO));

    polygon_free(circle);
}

// Weird nonconvex polygon
polygon_t *make_weird() {
    polygon_t *weird = polygon_init(5);
    list_t *w = get_points(weird);
    vector_t *v = malloc(sizeof(*v));
    *v = VEC_ZERO;
    list_add(w, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {4, 1};
    list_add(w, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {-2, 1};
    list_add(w, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {-5, 5};
    list_add(w, v);
    v = malloc(sizeof(*v));
    *v = (vector_t) {-1, -8};
    list_add(w, v);
    return weird;
}

void test_weird_area_centroid() {
    polygon_t *weird = make_weird();
    assert(isclose(polygon_area(weird), 23));
    assert(vec_isclose(polygon_centroid(weird), (vector_t) {-223.0 / 138.0, -51.0 / 46.0}));
    polygon_free(weird);
}

void test_weird_translate() {
    polygon_t *weird = make_weird();
    list_t *w = get_points(weird);
    polygon_translate(weird, (vector_t) {-10, -20});

    assert(vec_isclose(*(vector_t *)list_get(w, 0), (vector_t) {-10, -20}));
    assert(vec_isclose(*(vector_t *)list_get(w, 1), (vector_t) {-6,  -19}));
    assert(vec_isclose(*(vector_t *)list_get(w, 2), (vector_t) {-12, -19}));
    assert(vec_isclose(*(vector_t *)list_get(w, 3), (vector_t) {-15, -15}));
    assert(vec_isclose(*(vector_t *)list_get(w, 4), (vector_t) {-11, -28}));
    assert(isclose(polygon_area(weird), 23));
    assert(vec_isclose(polygon_centroid(weird), (vector_t) {-1603.0 / 138.0, -971.0 / 46.0}));

    polygon_free(weird);
}

void test_weird_rotate() {
    polygon_t *weird = make_weird();
    list_t *w = get_points(weird);
    // Rotate 90 degrees around (0, 2)
    polygon_rotate(weird, M_PI / 2, (vector_t) {0, 2});

    assert(vec_isclose(*(vector_t *)list_get(w, 0), (vector_t) {2,  2}));
    assert(vec_isclose(*(vector_t *)list_get(w, 1), (vector_t) {1,  6}));
    assert(vec_isclose(*(vector_t *)list_get(w, 2), (vector_t) {1,  0}));
    assert(vec_isclose(*(vector_t *)list_get(w, 3), (vector_t) {-3, -3}));
    assert(vec_isclose(*(vector_t *)list_get(w, 4), (vector_t) {10, 1}));
    assert(isclose(polygon_area(weird), 23));
    assert(vec_isclose(polygon_centroid(weird), (vector_t) {143.0 / 46.0, 53.0 / 138.0}));

    polygon_free(weird);
}

int main(int argc, char *argv[]) {
    // Run all tests? True if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_square_area_centroid)
    DO_TEST(test_square_translate)
    DO_TEST(test_square_rotate)
    DO_TEST(test_triangle_area_centroid)
    DO_TEST(test_triangle_translate)
    DO_TEST(test_triangle_rotate)
    DO_TEST(test_circ_area_centroid)
    DO_TEST(test_circ_translate)
    DO_TEST(test_circ_rotate)
    DO_TEST(test_weird_area_centroid)
    DO_TEST(test_weird_translate)
    DO_TEST(test_weird_rotate)

    puts("polygon_test PASS");
}
