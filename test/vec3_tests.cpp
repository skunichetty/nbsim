#include "unit_test_framework.h"
#include "vec3.h"
/**
 * Using a naive testing framework for now, a more robust framework is to come
 */

// testing proper vector addition and subtraction
TEST(test_vector_addition) {
    Vec3 vec1{1, 2, 3};
    Vec3 vec2{2, 3, 4};
    Vec3 vec3{-1, -2, -3};
    Vec3 vec4{0, 0, 0};

    Vec3 res1{3, 5, 7};

    ASSERT_EQUAL(vec1 + vec2, res1);         // basic test
    ASSERT_EQUAL(vec2 + vec1, res1);         // commutativity test
    ASSERT_EQUAL(vec1 + vec3, vec4);         // inverse test
    ASSERT_EQUAL(vec1 + vec4, vec1);         // identity test
    ASSERT_EQUAL(vec4 + vec3, vec3);         // identity test w/ commutative
    ASSERT_EQUAL(vec1 + vec2 + vec3, vec2);  // multiple operations
    ASSERT_EQUAL(vec2 + vec3,
                 vec2 - vec1);  // subtraction == addition of inverse
}

// testing proper vector addition with self assignment
TEST(test_vector_addition_self_assignment) {
    Vec3 vec1{1, 2, 3};
    Vec3 vec2{2, 3, 4};
    Vec3 vec3{-1, -2, -3};
    Vec3 vec4{0, 0, 0};
    Vec3 res1{3, 5, 7};
    Vec3 res2{2, 3, 4};
    Vec3 res3{1, 2, 3};

    vec1 += vec2;
    ASSERT_EQUAL(vec1, res1);  // basic test
    vec1 -= vec2;
    vec2 += vec1;
    ASSERT_EQUAL(vec2, res1);  // commutativity test
    vec2 -= vec1;
    vec1 += vec3;
    ASSERT_EQUAL(vec1, vec4);  // inverse test
    vec1 -= vec3;
    vec1 += vec4;
    ASSERT_EQUAL(vec1, res3);  // identity test
    vec4 += vec3;
    ASSERT_EQUAL(vec4, vec3);  // identity test w/ commutative
    vec4 -= vec3;
    vec1 += vec2 += vec3;
    ASSERT_EQUAL(vec1, res2);  // multiple operations
}

// testing proper multiplication and division
TEST(test_scalar_multiplication) {
    Vec3 vec1{1, 2, 3};
    Vec3 vec2{2, 3, 4};
    Vec3 vec4{0, 0, 0};

    Vec3 res1{2, 4, 6};
    Vec3 res2{-3, -6, -9};
    Vec3 res3{40, 60, 80};

    ASSERT_EQUAL(vec1 * 2, res1);   // basic test
    ASSERT_EQUAL(vec1 * -3, res2);  // flip vector direction
    ASSERT_EQUAL(vec1 * 0, vec4);   // multiply by zero
    ASSERT_EQUAL(vec2 * 1, vec2);   // identity test
    ASSERT_EQUAL(vec2 * 1 / float(5),
                 vec2 / 5);  // assert division is the same as multiplication
    ASSERT_EQUAL(vec2 * -5 * -4, res3);  // multiple operations
}

// testing proper multiplication and division with self assignment
TEST(test_scalar_multiplication_self_assignment) {
    Vec3 vec1{1, 2, 3};
    Vec3 vec2{2, 3, 4};
    Vec3 vec4{0, 0, 0};

    Vec3 res1{2, 4, 6};
    Vec3 res2{-3, -6, -9};
    Vec3 res3{2, 3, 4};
    Vec3 res4{40, 60, 80};

    vec1 *= 2;
    ASSERT_EQUAL(vec1, res1);  // basic test
    vec1 /= 2;
    vec1 *= -3;
    ASSERT_EQUAL(vec1, res2);  // flip vector direction
    vec1 /= -3;
    vec1 *= 0;
    ASSERT_EQUAL(vec1, vec4);  // multiply by zero
    vec1 = Vec3{1, 2, 3};
    vec2 *= 1;
    ASSERT_EQUAL(vec2, res3);  // identity test
    (vec2 *= -5) *= -4;
    ASSERT_EQUAL(vec2, res4);  // multiple operations
}

// testing the ability to correctly return length
TEST(test_vector_length) {
    Vec3 vec1{1, 2, 2};
    Vec3 vec2{0, 0, 0};
    Vec3 vec3{-3, -6, -9};
    Vec3 vec4{40, 60, 80};

    ASSERT_ALMOST_EQUAL(vec1.length(), 3, 0.000001);  // basic test
    ASSERT_ALMOST_EQUAL(vec2.length(), 0, 0.000001);  // zero vector length
    ASSERT_ALMOST_EQUAL(vec3.length(), 11.22497218,
                        0.000001);  // average case test
    ASSERT_ALMOST_EQUAL(vec4.length(), 107.7032961,
                        0.000001);  // another average case test
}

TEST_MAIN()