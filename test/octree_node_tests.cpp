#include "octree_node.h"
#include "unit_test_framework.h"

// tests to see if the node properly intializes
TEST(test_basic_init) {
    Vec3 center{0, 0, 0};
    OctreeNode node(1000, center);
    ASSERT_EQUAL(node.getBounds().width, 1000.0f);
    ASSERT_EQUAL(node.getType(), OctreeNodeType::EXTERNAL);
    Object obj(10, Vec3{1, 0, 0});
    Object obj2(10, Vec3{-1, -1, -1});
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
    Object obj(10, Vec3{1, 0, 0});
    Object obj2(10, Vec3{-1, -1, -1});
    Object obj3(10, Vec3{1, 1, -1});
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
    Object obj(10, Vec3{1, 0, 0});
    Object obj2(10, Vec3{-1, -1, -1});
    Object obj3(10, Vec3{1, 1, -1});
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
    Object obj(10, Vec3{1, 0, 0});
    Object obj2(10, Vec3{-1, -1, -1});
    Object obj3(10, Vec3{1, 1, -1});
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
    Object obj(10, Vec3{1, 0, 0});
    Object obj2(10, Vec3{-1, -1, -1});
    Object obj3(10, Vec3{13.5, 0, 0});
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
    Object obj(10, Vec3{1, 0, 0});
    Object obj2(10, Vec3{-1, -1, -1});
    Object obj3(10, Vec3{13.5, 0, 0});
    Vec3 res1{4.5, -1 / float(3), -1 / float(3)};
    Vec3 res2{7.25, 0, 0};
    node.insert(&obj);
    node.insert(&obj2);
    node.insert(&obj3);
    // fuzzy compare
    ASSERT_ALMOST_EQUAL(node.getObject().position.x, res1.x, 0.00001);
    ASSERT_ALMOST_EQUAL(node.getObject().position.y, res1.y, 0.00001);
    ASSERT_ALMOST_EQUAL(node.getObject().position.z, res1.z, 0.00001);
    // fuzzy compare
    ASSERT_ALMOST_EQUAL(node.children[0]->getObject().position.x, res2.x,
                        0.00001);
    ASSERT_ALMOST_EQUAL(node.children[0]->getObject().position.y, res2.y,
                        0.00001);
    ASSERT_ALMOST_EQUAL(node.children[0]->getObject().position.z, res2.z,
                        0.00001);
}

// tests access to objects, specifically error checking.
TEST(test_object_accessor) {
    Vec3 center{0, 0, 0};
    OctreeNode node(100, center);
    Object obj(10, Vec3{1, 0, 0});
    Object obj2(10, Vec3{-1, -1, -1});
    Object obj3(10, Vec3{13.5, 0, 0});
    try {
        node.getObject();
    } catch (...) {
        std::cout << "error caught" << std::endl;
    }
    ASSERT_TRUE(node.empty());
    node.insert(&obj);
    node.insert(&obj2);
    node.insert(&obj3);
    ASSERT_FALSE(node.empty());
}

TEST_MAIN()