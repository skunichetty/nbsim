#include <sstream>

#include "octree.h"
#include "unit_test_framework.h"

using namespace std;

// tests to make sure basic initialization works
TEST(test_basic_initialization) {
    ostringstream os;
    ostringstream os2;
    Octree tree(1000);
    tree.printSummary(os);
    Object obj{10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1}};
    Object obj2{10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1}};
    Object obj3{10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1}};
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
    Object obj{10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1}};
    Object obj2{10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1}};
    Object obj3{10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1}};
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
TEST(test_assignment_op) {
    ostringstream os;
    ostringstream os2;
    ostringstream os3;
    Octree tree(1000);
    Object obj{10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1}};
    Object obj2{10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1}};
    Object obj3{10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1}};
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

TEST_MAIN()