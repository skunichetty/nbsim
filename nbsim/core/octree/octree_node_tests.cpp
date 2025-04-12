#include "nbsim/core/octree/octree_node.hpp"
#include <gtest/gtest.h>

class TestOctreeNode : public ::testing::Test {
protected:
  TestOctreeNode() = default;
};

TEST_F(TestOctreeNode, InsertsObjectIntoNodeWhenEmpty) {
  Vec3 center{0, 0, 0};
  OctreeNode node(1000, center);
  Object obj(10, Vec3{1, 0, 0});
  node.insert(&obj);
  EXPECT_EQ(node.children[7], nullptr);
}

TEST_F(TestOctreeNode, InsertsObjectIntoNodeWhenHasOneOtherObject) {
  Vec3 center{0, 0, 0};
  OctreeNode node(1000, center);
  Object obj(10, Vec3{1, 0, 0});
  Object obj2(10, Vec3{-1, -1, -1});
  node.insert(&obj);
  node.insert(&obj2);
  EXPECT_NE(node.children[0], nullptr);
  EXPECT_NE(node.children[7], nullptr);
}

TEST_F(TestOctreeNode, InsertsObjectIntoNodeWhenHasSubnodes) {
  Vec3 center{0, 0, 0};
  OctreeNode node(1000, center);
  Object obj(10, Vec3{1, 0, 0});
  Object obj2(10, Vec3{-1, -1, -1});
  Object obj3(1, Vec3{1, -1, 1});
  node.insert(&obj);
  node.insert(&obj2);
  node.insert(&obj3);
  EXPECT_NE(node.children[7], nullptr);
  EXPECT_NE(node.children[0], nullptr);
  EXPECT_NE(node.children[2], nullptr);
}
