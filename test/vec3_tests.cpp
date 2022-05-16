#include <sstream>
#include <string>

#include "catch.h"
#include "vec3.h"

TEST_CASE("Tests basic vector addition") {
    Vec3 vec1{1, 2, 3};
    Vec3 vec2{2, 3, 4};
    Vec3 result{3, 5, 7};
    REQUIRE(vec1 + vec2 == result);
}

TEST_CASE("Tests basic vector addition with self-assignment") {
    Vec3 vec{1, 2, 3};
    vec += Vec3{2, 3, 4};
    Vec3 result{3, 5, 7};
    REQUIRE(vec == result);
}

TEST_CASE("Tests correctness of commutative addition in vector space") {
    Vec3 vec1{1, 2, 3};
    Vec3 vec2{2, 3, 4};
    REQUIRE(vec2 + vec1 == vec1 + vec2);
}

TEST_CASE("Tests correctness of commutative addition with self-assignment") {
    Vec3 vec1{1, 2, 3};
    Vec3 vec2{1, 2, 3};
    Vec3 addend{2, 3, 4};
    vec1 += addend;
    addend += vec2;
    REQUIRE(vec1 == addend);
}

TEST_CASE("Tests correctness of additive inverse in vector space") {
    Vec3 vec1{1, 2, 3};
    Vec3 vec2{-1, -2, -3};
    Vec3 result{0, 0, 0};
    REQUIRE(vec2 + vec1 == result);
}

TEST_CASE("Tests correctness of additive inverse with self-assignment") {
    Vec3 vec{1, 2, 3};
    vec += Vec3{-1, -2, -3};
    Vec3 result{0, 0, 0};
    REQUIRE(vec == result);
}

TEST_CASE("Tests correctness of identity under addition in vector space") {
    Vec3 vec1{1, 2, 3};
    Vec3 vec2{0, 0, 0};
    Vec3 result{1, 2, 3};
    REQUIRE(vec2 + vec1 == result);
}

TEST_CASE("Tests correctness of identity under addition with self-assignment") {
    Vec3 vec{1, 2, 3};
    vec += Vec3{0, 0, 0};
    Vec3 result{1, 2, 3};
    REQUIRE(vec == result);
}

TEST_CASE("Tests addition of multiple vectors together") {
    Vec3 vec1{1, 2, 3};
    Vec3 vec2{2, 3, 4};
    Vec3 vec3{-1, -2, -3};
    REQUIRE(vec1 + vec2 + vec3 == vec2);
}

TEST_CASE("Tests addition of multiple vectors with self-assignment") {
    Vec3 vec1{1, 2, 3};
    Vec3 vec2{2, 3, 4};
    Vec3 vec3{-1, -2, -3};
    Vec3 result{2, 3, 4};
    vec1 += (vec2 += vec3);
    REQUIRE(vec1 == result);
}

TEST_CASE("Tests correctness of subtraction in comparison to addition") {
    Vec3 vec1{1, 2, 3};
    Vec3 vec2{2, 3, 4};
    Vec3 vec3{-1, -2, -3};

    Vec3 res1 = vec2 + vec3;
    Vec3 res2 = vec2 - vec1;
    REQUIRE(res1 == res2);
}

TEST_CASE("Tests vector direction flip") {
    Vec3 vec1{1, 2, 3};
    Vec3 vec2{-1, -2, -3};
    REQUIRE(vec2 == -1 * vec1);
}

TEST_CASE("Tests vector direction flip with self-assignment") {
    Vec3 vec1{1, 2, 3};
    vec1 *= -1;
    Vec3 vec2{-1, -2, -3};
    REQUIRE(vec2 == vec1);
}

TEST_CASE("Tests multiplication by zero") {
    Vec3 vec{1, 2, 3};
    Vec3 result{0, 0, 0};
    REQUIRE(vec * 0 == result);
}

TEST_CASE("Tests multiplication by zero with self-assignment") {
    Vec3 vec{1, 2, 3};
    vec *= 0;
    Vec3 result{0, 0, 0};
    REQUIRE(vec == result);
}

TEST_CASE("Tests correctness of multiplicative identity in vector space") {
    Vec3 vec{1, 2, 3};
    REQUIRE(vec * 1 == vec);
}

TEST_CASE("Tests correctness of multiplicative identity with self-assignment") {
    Vec3 vec{1, 2, 3};
    Vec3 result{1, 2, 3};
    vec *= 1;
    REQUIRE(vec == result);
}

TEST_CASE("Tests chained scalar multiplication operations") {
    Vec3 vec{1, 2, 3};
    Vec3 result{20, 40, 60};
    REQUIRE(vec * -5 * -4 == result);
}

TEST_CASE("Tests equality between fractional multiplication and division") {
    Vec3 vec{20, 15, 10};
    double factor = 1 / static_cast<double>(5);
    Vec3 multiplication_result = vec * factor;
    Vec3 division_result = vec / 5;
    REQUIRE(multiplication_result.x == Approx(division_result.x));
    REQUIRE(multiplication_result.y == Approx(division_result.y));
    REQUIRE(multiplication_result.z == Approx(division_result.z));
}

TEST_CASE("Tests that correct length is returned (Square root of l2 norm)") {
    Vec3 vec1{1, 2, 2};
    Vec3 vec2{0, 0, 0};
    Vec3 vec3{-3, -6, -9};
    Vec3 vec4{40, 60, 80};

    const double EPSILON = 0.0001;

    REQUIRE(vec1.length() == Approx(3));
    REQUIRE(vec2.length() == Approx(0));
    REQUIRE(vec3.length() == Approx(11.22497218));
    REQUIRE(vec4.length() == Approx(107.7032961));
}

TEST_CASE("Tests inequality between two vectors") {
    Vec3 vec{1, 2, 3};
    Vec3 result{20, 40, 60};
    REQUIRE(vec != result);
}

TEST_CASE("Tests correctness of debug printout of vector") {
    const std::string result = "[20,-40,60]";
    std::ostringstream os;
    Vec3 vec{20, -40, 60};
    os << vec;
    REQUIRE(os.str() == result);
}