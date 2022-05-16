#include "catch.h"
#include "octree_node.h"

TEST_CASE("Inserts object into node when empty") {
    Vec3 center{0, 0, 0};
    OctreeNode node(1000, center, nullptr);
    Object obj(10, Vec3{1, 0, 0});
    node.insert(&obj);
    REQUIRE(node.children[7] == nullptr);
}

TEST_CASE("Inserts object into node when has one other object") {
    Vec3 center{0, 0, 0};
    OctreeNode node(1000, center, nullptr);
    Object obj(10, Vec3{1, 0, 0});
    Object obj2(10, Vec3{-1, -1, -1});
    node.insert(&obj);
    node.insert(&obj2);
    REQUIRE(node.object == nullptr);
    REQUIRE(node.children[0] != nullptr);
}

TEST_CASE("Inserts object into node when has subnodes") {
    Vec3 center{0, 0, 0};
    OctreeNode node(1000, center, nullptr);
    Object obj(10, Vec3{1, 0, 0});
    Object obj2(10, Vec3{-1, -1, -1});
    Object obj3(1, Vec3{1, -1, 1});
    node.insert(&obj);
    node.insert(&obj2);
    node.insert(&obj3);
    REQUIRE(node.children[7] != nullptr);
    REQUIRE(node.children[0] != nullptr);
    REQUIRE(node.children[2] != nullptr);
}

TEST_CASE("Removes object when node has one object") {
    Vec3 center{0, 0, 0};
    OctreeNode node(1000, center, nullptr);
    Object obj(10, Vec3{1, 0, 0});
    node.insert(&obj);
    node.remove();
    REQUIRE(node.object == nullptr);
}

TEST_CASE("Removes object when node has subnodes") {
    Vec3 center{0, 0, 0};
    OctreeNode node(1000, center, nullptr);
    Object obj(10, Vec3{1, 0, 0});
    Object obj2(10, Vec3{-1, -1, -1});
    Object obj3(1, Vec3{1, -1, 1});
    node.insert(&obj);
    node.insert(&obj2);
    node.insert(&obj3);
    node.remove();
    REQUIRE(node.children[7] != nullptr);
    REQUIRE(node.children[0] != nullptr);
    REQUIRE(node.children[2] != nullptr);
}

TEST_CASE("Checks if node correctly returns empty state") {
    Vec3 center{0, 0, 0};
    OctreeNode node(1000, center, nullptr);
    Object obj(10, Vec3{1, 0, 0});
    REQUIRE(node.empty());
    node.insert(&obj);
    REQUIRE(!node.empty());
    node.remove();
    REQUIRE(node.empty());
}

TEST_CASE("Checks if node parent is correctly created on subdivision") {
    Vec3 center{0, 0, 0};
    OctreeNode node(1000, center, nullptr);
    Object obj(10, Vec3{1, 0, 0});
    Object obj2(10, Vec3{-1, -1, -1});
    node.insert(&obj);
    node.insert(&obj2);
    REQUIRE(node.children[7]->parent == &node);
    REQUIRE(node.children[0]->parent == &node);
}

TEST_CASE("Check if entire subtree is pruned when it loses all its nodes") {
    return;
}

TEST_CASE("Checks if node is fixed upon invariant break") {
    OctreeNode node(1000, Vec3{0, 0, 0}, nullptr);
    Object obj(10, Vec3{-1, 2, -1});
    Object obj2(10, Vec3{-1, -1, -1});
    node.insert(&obj);
    node.insert(&obj2);
    REQUIRE(node.children[0] == nullptr);
    obj2.position = Vec3{1, 1, 1};  // move to octant 1
    node.update();
    REQUIRE(node.children[0] != nullptr);
}

TEST_CASE("Checks if branch is pruned when empty") {
    OctreeNode node(1000, Vec3{0, 0, 0}, nullptr);
    Object obj(10, Vec3{-1, 2, -1});
    Object obj2(10, Vec3{-1, -1, -1});
    node.insert(&obj);
    node.insert(&obj2);
    REQUIRE(node.children[0] == nullptr);
    obj2.position = Vec3{1, 1, 1};  // move to octant 1
    for (size_t i = 0; i < 9; ++i) {
        node.update();
    }
    REQUIRE(node.children[0] != nullptr);
    REQUIRE(node.children[7] == nullptr);
}