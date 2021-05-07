#include "octree_node.h"
#include "unit_test_framework.h"

// tests to see if the node properly intializes
TEST(test_basic_init) {
    Vec3 center{0, 0, 0};
    OctreeNode node(1000, center);
    ASSERT_EQUAL(node.width, 1000);
    ASSERT_EQUAL(node.type, OctreeNodeType::EXTERNAL);
    Object obj{10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1}};
    Object obj2{10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1}};
    node.insert(&obj);
    ASSERT_EQUAL(node.children[7], nullptr);
    node.insert(&obj2);
    ASSERT_NOT_EQUAL(node.children[7], nullptr);
    ASSERT_NOT_EQUAL(node.children[0], nullptr);
}

// tests ability to make deep copies of each node
TEST(test_copy_ctor) {
    Vec3 center{0, 0, 0};
    OctreeNode node(1000, center);
    Object obj{10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1}};
    Object obj2{10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1}};
    Object obj3{10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1}};
    node.insert(&obj);
    node.insert(&obj2);
    OctreeNode node2 = node;
    node2.insert(&obj3);
    ASSERT_NOT_EQUAL(node2.children[1], nullptr);
    ASSERT_EQUAL(node.children[1], nullptr);
}

// tests ability to reassign nodes properly w/o memory being lost
TEST(test_assignment_op) {
    Vec3 center{0, 0, 0};
    OctreeNode node(1000, center);
    Object obj{10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1}};
    Object obj2{10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1}};
    Object obj3{10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1}};
    node.insert(&obj);
    node.insert(&obj2);
    OctreeNode node2(500, center);
    node2.insert(&obj3);
    node2 = node;
    ASSERT_NOT_EQUAL(node2.children[7], nullptr);
    ASSERT_NOT_EQUAL(node2.children[0], nullptr);
    node2.insert(&obj3);
    ASSERT_NOT_EQUAL(node2.children[1], nullptr);
    ASSERT_EQUAL(node.children[1], nullptr);
}

// tests the move constructor
TEST(test_move_ctor) {
    Vec3 center{0, 0, 0};
    OctreeNode node(1000, center);
    Object obj{10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1}};
    Object obj2{10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1}};
    Object obj3{10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1}};
    node.insert(&obj);
    node.insert(&obj2);
    OctreeNode node2 = std::move(node);
    node2.insert(&obj3);
    ASSERT_NOT_EQUAL(node2.children[1], nullptr);
}

// tests multiple insertions into the same quadrant spawns children correctly
TEST(test_multiple_insertion) {
    Vec3 center{0, 0, 0};
    OctreeNode node(100, center);
    Object obj{10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1}};
    Object obj2{10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1}};
    Object obj3{10, Vec3{13.5, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1}};
    node.insert(&obj);
    node.insert(&obj2);
    node.insert(&obj3);
    ASSERT_NOT_EQUAL(node.children[0], nullptr);
    ASSERT_NOT_EQUAL(node.children[0]->children[7], nullptr);
    ASSERT_NOT_EQUAL(node.children[0]->children[7]->children[3], nullptr);
}

// tests correct centers of mass are being calculated
TEST(test_center_of_mass) {
    Vec3 center{0, 0, 0};
    OctreeNode node(100, center);
    Object obj{10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1}};
    Object obj2{10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1}};
    Object obj3{10, Vec3{13.5, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1}};
    Vec3 res1{4.5, -1 / float(3), -1 / float(3)};
    Vec3 res2{7.25, 0, 0};
    node.insert(&obj);
    node.insert(&obj2);
    node.insert(&obj3);
    // fuzzy compare
    ASSERT_ALMOST_EQUAL(node.localObj->position.x, res1.x, 0.00001);
    ASSERT_ALMOST_EQUAL(node.localObj->position.y, res1.y, 0.00001);
    ASSERT_ALMOST_EQUAL(node.localObj->position.z, res1.z, 0.00001);
    // fuzzy compare
    ASSERT_ALMOST_EQUAL(node.children[0]->localObj->position.x, res2.x,
                        0.00001);
    ASSERT_ALMOST_EQUAL(node.children[0]->localObj->position.y, res2.y,
                        0.00001);
    ASSERT_ALMOST_EQUAL(node.children[0]->localObj->position.z, res2.z,
                        0.00001);
}

TEST_MAIN()