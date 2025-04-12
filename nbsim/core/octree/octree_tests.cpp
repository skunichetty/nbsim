#include "nbsim/core/octree/octree.hpp"
#include <gtest/gtest.h>
#include <sstream>
#include <stack>

using namespace std;

class TestOctree : public ::testing::Test {
protected:
  TestOctree() = default;
};

TEST_F(TestOctree, BasicInitialization) {
  ostringstream os;
  ostringstream os2;
  Octree tree;
  tree.printSummary(os);
  Body obj(10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
  Body obj2(10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
  Body obj3(10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
  tree.insert(obj);
  tree.insert(obj2);
  tree.insert(obj3);
  tree.printSummary(os2);
  EXPECT_NE(os.str(), os2.str());
}

TEST_F(TestOctree, DeepCopyWithCopyConstructor) {
  ostringstream os;
  ostringstream os2;
  ostringstream os3;
  Octree tree;
  Body obj(10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
  Body obj2(10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
  Body obj3(10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
  tree.insert(obj);
  tree.insert(obj2);
  Octree tree2 = tree;
  tree.printSummary(os);
  tree2.printSummary(os2);
  EXPECT_EQ(os.str(), os2.str());
  tree2.insert(obj3);
  tree2.printSummary(os3);
  EXPECT_NE(os.str(), os3.str());
}

TEST_F(TestOctree, AssignmentOperatorCorrectness) {
  ostringstream os;
  ostringstream os2;
  ostringstream os3;
  Octree tree;
  Body obj(10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
  Body obj2(10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
  Body obj3(10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
  tree.insert(obj);
  tree.insert(obj2);
  Octree tree2;
  tree2.insert(obj3);
  tree.printSummary(os);
  tree2.printSummary(os2);
  EXPECT_NE(os.str(), os2.str());
  tree = tree2;
  tree.printSummary(os3);
  EXPECT_EQ(os2.str(), os3.str());
}

// TEST_F(TestOctree, VariableLengthWidth) {
//   Octree tree;
//   Body obj(10, Vec3{10000, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
//   Body obj2(10, Vec3{10000, -8500, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
//   Body obj3(10, Vec3{0, 1, -25000}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
//   EXPECT_EQ(tree.root, nullptr);
//   tree.insert(obj);
//   EXPECT_FLOAT_EQ(tree.root->getBounds().width, 30000.0f);
//   tree.insert(obj2);
//   EXPECT_FLOAT_EQ(tree.root->getBounds().width, 30000.0f);
//   tree.insert(obj3);
//   EXPECT_FLOAT_EQ(tree.root->getBounds().width, 75000.0f);
// }

TEST_F(TestOctree, IteratorsLoopThroughAllObjects) {
  Body obj(10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
  Body obj2(10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
  Body obj3(10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
  vector<Body> bodies({obj, obj2, obj3});
  Octree tree(bodies);
  size_t index = 0;
  for (auto &body : tree) {
    EXPECT_EQ(body.mass, bodies[index].mass);
    EXPECT_EQ(body.position, bodies[index++].position);
  }
}

TEST_F(TestOctree, IteratorEqualityOperation) {
  Body obj(10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
  Body obj2(10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
  Body obj3(10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
  vector<Body> bodies({obj, obj2, obj3});
  Octree tree(bodies);
  auto it = tree.begin();
  auto it2 = tree.begin();
  EXPECT_EQ(it, it2);
  it2++;
  EXPECT_NE(it, it2);
  ++it;
  EXPECT_EQ(it, it2);
}

TEST_F(TestOctree, BoundsAdaptToWidthWhileBuilding) {
  Body obj(10000, Vec3{1000, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
  Body obj2(10000, Vec3{-1, -1000, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
  Body obj3(10000, Vec3{1, 2005, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
  vector<Body> bodies({obj, obj2, obj3});
  Octree tree;
  for (Body &obj : bodies) {
    tree.insert(obj);
  }
  tree.buildTree();
}
