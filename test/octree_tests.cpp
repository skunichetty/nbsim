#include <sstream>
#include <stack>

#include "octree.h"
#include "unit_test_framework.h"

using namespace std;

// tests to make sure basic initialization works
TEST(test_basic_initialization) {
    ostringstream os;
    ostringstream os2;
    Octree tree(1000);
    tree.printSummary(os);
    Body obj(10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
    Body obj2(10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    Body obj3(10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    tree.insert(obj);
    tree.insert(obj2);
    tree.insert(obj3);
    tree.printSummary(os2);
    ASSERT_NOT_EQUAL(os.str(), os2.str());
}
// test that copy ctor works properly
TEST(test_copy_ctor) {
    ostringstream os;
    ostringstream os2;
    ostringstream os3;
    Octree tree(1000);
    Body obj(10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
    Body obj2(10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    Body obj3(10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    tree.insert(obj);
    tree.insert(obj2);
    Octree tree2 = tree;
    tree.printSummary(os);
    tree2.printSummary(os2);
    ASSERT_EQUAL(os.str(), os2.str());
    tree2.insert(obj3);
    tree2.printSummary(os3);
    ASSERT_NOT_EQUAL(os.str(), os3.str());
}

// test that assignment operator works properly
// implicitly tests move assignment
TEST(test_assignment_op) {
    ostringstream os;
    ostringstream os2;
    ostringstream os3;
    Octree tree(1000);
    Body obj(10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
    Body obj2(10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    Body obj3(10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    tree.insert(obj);
    tree.insert(obj2);
    Octree tree2(500);
    tree2.insert(obj3);
    // check that they arent equal
    tree.printSummary(os);
    tree2.printSummary(os2);
    ASSERT_NOT_EQUAL(os.str(), os2.str());
    tree = tree2;
    // check that they are equal now
    tree.printSummary(os3);
    ASSERT_EQUAL(os2.str(), os3.str());
}

// tests that tree adjusts width appropriately for variable length widths
TEST(test_adaptive_width) {
    Octree tree;
    Body obj(10, Vec3{10000, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
    Body obj2(10, Vec3{10000, -8500, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    Body obj3(10, Vec3{0, 1, -25000}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    tree.insert(obj);
    ASSERT_EQUAL(tree.root->getBounds().width, 30000.0f);
    tree.insert(obj2);
    ASSERT_EQUAL(tree.root->getBounds().width, 30000.0f);
    tree.insert(obj3);
    ASSERT_EQUAL(tree.root->getBounds().width, 75000.0f);
}

// tests that iterators loop through objects properly
TEST(test_iterator) {
    Body obj(10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
    Body obj2(10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    Body obj3(10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    vector<Body> bodies({obj, obj2, obj3});
    Octree tree(bodies);
    size_t index = 0;
    for (auto& body : tree) {
        ASSERT_EQUAL(body.mass, bodies[index].mass);
        ASSERT_EQUAL(body.position, bodies[index++].position);
    }
}

// test iterator equality
TEST(test_iterator_equality) {
    Body obj(10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
    Body obj2(10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    Body obj3(10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    vector<Body> bodies({obj, obj2, obj3});
    Octree tree(bodies);
    auto it = tree.begin();
    auto it2 = tree.begin();
    ASSERT_EQUAL(it, it2);
    it2++;
    ASSERT_NOT_EQUAL(it, it2);
    ++it;
    ASSERT_EQUAL(it, it2);
}

// tests that nodes of the tree can be traversed through root of octree
TEST(test_tree_traversal) {
    stack<OctreeNode*> s;
    Body obj(10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
    Body obj2(10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    Body obj3(10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    vector<Body> bodies({obj, obj2, obj3});
    Octree tree(bodies);
    s.push(tree.root);
    while (!s.empty()) {
        auto current = s.top();
        s.pop();
        if (current->getType() == OctreeNodeType::EXTERNAL) {
            if (!current->empty()) {
                ASSERT_FALSE(current->empty());
                ASSERT_EQUAL(current->getObject().mass, 10.0f);
            } else {
                ASSERT_TRUE(current->empty());
            }
        } else {
            ASSERT_NOT_EQUAL(current->getObject().mass, 10.0f);
        }
        for (auto child : current->children) {
            if (child) {
                s.push(child);
            }
        }
    }
}

// Tests that bounds adapt to width while building. Will not segfault if
// proper.
TEST(test_adaptive_bounds) {
    Body obj(10000, Vec3{1000, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
    Body obj2(10000, Vec3{-1, -1000, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    Body obj3(10000, Vec3{1, 2005, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    vector<Body> bodies({obj, obj2, obj3});
    Octree tree(10);
    for (Body& obj : bodies) {
        tree.insert(obj);
    }
    tree.buildTree();
}

TEST_MAIN()