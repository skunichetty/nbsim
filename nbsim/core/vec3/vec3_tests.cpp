#include <sstream>
#include <string>

#include "nbsim/core/vec3/vec3.hpp"
#include <gtest/gtest.h>

class Vec3Test : public ::testing::Test {
protected:
  Vec3Test() = default;
};

TEST_F(Vec3Test, BasicVectorAddition) {
  Vec3 vec1{1, 2, 3};
  Vec3 vec2{2, 3, 4};
  Vec3 result{3, 5, 7};
  EXPECT_EQ(vec1 + vec2, result);
}

TEST_F(Vec3Test, BasicVectorAdditionWithSelfAssignment) {
  Vec3 vec{1, 2, 3};
  vec += Vec3{2, 3, 4};
  Vec3 result{3, 5, 7};
  EXPECT_EQ(vec, result);
}

TEST_F(Vec3Test, CommutativeAddition) {
  Vec3 vec1{1, 2, 3};
  Vec3 vec2{2, 3, 4};
  EXPECT_EQ(vec2 + vec1, vec1 + vec2);
}

TEST_F(Vec3Test, CommutativeAdditionWithSelfAssignment) {
  Vec3 vec1{1, 2, 3};
  Vec3 vec2{1, 2, 3};
  Vec3 addend{2, 3, 4};
  vec1 += addend;
  addend += vec2;
  EXPECT_EQ(vec1, addend);
}

TEST_F(Vec3Test, AdditiveInverse) {
  Vec3 vec1{1, 2, 3};
  Vec3 vec2{-1, -2, -3};
  Vec3 result{0, 0, 0};
  EXPECT_EQ(vec2 + vec1, result);
}

TEST_F(Vec3Test, AdditiveInverseWithSelfAssignment) {
  Vec3 vec{1, 2, 3};
  vec += Vec3{-1, -2, -3};
  Vec3 result{0, 0, 0};
  EXPECT_EQ(vec, result);
}

TEST_F(Vec3Test, IdentityUnderAddition) {
  Vec3 vec1{1, 2, 3};
  Vec3 vec2{0, 0, 0};
  Vec3 result{1, 2, 3};
  EXPECT_EQ(vec2 + vec1, result);
}

TEST_F(Vec3Test, IdentityUnderAdditionWithSelfAssignment) {
  Vec3 vec{1, 2, 3};
  vec += Vec3{0, 0, 0};
  Vec3 result{1, 2, 3};
  EXPECT_EQ(vec, result);
}

TEST_F(Vec3Test, AdditionOfMultipleVectors) {
  Vec3 vec1{1, 2, 3};
  Vec3 vec2{2, 3, 4};
  Vec3 vec3{-1, -2, -3};
  EXPECT_EQ(vec1 + vec2 + vec3, vec2);
}

TEST_F(Vec3Test, AdditionOfMultipleVectorsWithSelfAssignment) {
  Vec3 vec1{1, 2, 3};
  Vec3 vec2{2, 3, 4};
  Vec3 vec3{-1, -2, -3};
  Vec3 result{2, 3, 4};
  vec1 += (vec2 += vec3);
  EXPECT_EQ(vec1, result);
}

TEST_F(Vec3Test, SubtractionComparisonToAddition) {
  Vec3 vec1{1, 2, 3};
  Vec3 vec2{2, 3, 4};
  Vec3 vec3{-1, -2, -3};

  Vec3 res1 = vec2 + vec3;
  Vec3 res2 = vec2 - vec1;
  EXPECT_EQ(res1, res2);
}

TEST_F(Vec3Test, VectorDirectionFlip) {
  Vec3 vec1{1, 2, 3};
  Vec3 vec2{-1, -2, -3};
  EXPECT_EQ(vec2, vec1 * -1);
}

TEST_F(Vec3Test, VectorDirectionFlipWithSelfAssignment) {
  Vec3 vec1{1, 2, 3};
  vec1 *= -1;
  Vec3 vec2{-1, -2, -3};
  EXPECT_EQ(vec2, vec1);
}

TEST_F(Vec3Test, MultiplicationByZero) {
  Vec3 vec{1, 2, 3};
  Vec3 result{0, 0, 0};
  EXPECT_EQ(vec * 0, result);
}

TEST_F(Vec3Test, MultiplicationByZeroWithSelfAssignment) {
  Vec3 vec{1, 2, 3};
  vec *= 0;
  Vec3 result{0, 0, 0};
  EXPECT_EQ(vec, result);
}

TEST_F(Vec3Test, MultiplicativeIdentity) {
  Vec3 vec{1, 2, 3};
  EXPECT_EQ(vec * 1, vec);
}

TEST_F(Vec3Test, MultiplicativeIdentityWithSelfAssignment) {
  Vec3 vec{1, 2, 3};
  Vec3 result{1, 2, 3};
  vec *= 1;
  EXPECT_EQ(vec, result);
}

TEST_F(Vec3Test, ChainedScalarMultiplication) {
  Vec3 vec{1, 2, 3};
  Vec3 result{20, 40, 60};
  EXPECT_EQ(vec * -5 * -4, result);
}

TEST_F(Vec3Test, FractionalMultiplicationAndDivision) {
  Vec3 vec{20, 15, 10};
  double factor = 1 / static_cast<double>(5);
  Vec3 multiplication_result = vec * factor;
  Vec3 division_result = vec / 5;
  EXPECT_NEAR(multiplication_result.x, division_result.x, 0.0001);
  EXPECT_NEAR(multiplication_result.y, division_result.y, 0.0001);
  EXPECT_NEAR(multiplication_result.z, division_result.z, 0.0001);
}

TEST_F(Vec3Test, LengthCalculation) {
  Vec3 vec1{1, 2, 2};
  Vec3 vec2{0, 0, 0};
  Vec3 vec3{-3, -6, -9};
  Vec3 vec4{40, 60, 80};

  EXPECT_NEAR(vec1.length(), 3, 0.0001);
  EXPECT_NEAR(vec2.length(), 0, 0.0001);
  EXPECT_NEAR(vec3.length(), 11.22497218, 0.0001);
  EXPECT_NEAR(vec4.length(), 107.7032961, 0.0001);
}

TEST_F(Vec3Test, InequalityBetweenVectors) {
  Vec3 vec{1, 2, 3};
  Vec3 result{20, 40, 60};
  EXPECT_NE(vec, result);
}

TEST_F(Vec3Test, DebugPrintout) {
  const std::string result = "[20,-40,60]";
  std::ostringstream os;
  Vec3 vec{20, -40, 60};
  os << vec;
  EXPECT_EQ(os.str(), result);
}
