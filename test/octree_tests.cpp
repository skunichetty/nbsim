#include <sstream>
#include <stack>

#include "catch.h"
#include "octree.h"

using namespace std;

TEST_CASE("Tests the basic initialization of the Octree") {
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
    REQUIRE(os.str() != os2.str());
}

TEST_CASE("Tests that deep copying with the copy constructor") {
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
    REQUIRE(os.str() == os2.str());
    tree2.insert(obj3);
    tree2.printSummary(os3);
    REQUIRE(os.str() != os3.str());
}

TEST_CASE("Tests the correctness of the assignment operator") {
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
    // check that they arent equal
    tree.printSummary(os);
    tree2.printSummary(os2);
    REQUIRE(os.str() != os2.str());
    tree = tree2;
    // check that they are equal now
    tree.printSummary(os3);
    REQUIRE(os2.str() == os3.str());
}

TEST_CASE("Tests variable length width of octree") {
    Octree tree;
    Body obj(10, Vec3{10000, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
    Body obj2(10, Vec3{10000, -8500, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    Body obj3(10, Vec3{0, 1, -25000}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    tree.insert(obj);
    REQUIRE(tree.root->bounds.width == Approx(30000.0f));
    tree.insert(obj2);
    REQUIRE(tree.root->bounds.width == Approx(30000.0f));
    tree.insert(obj3);
    REQUIRE(tree.root->bounds.width == Approx(75000.0f));
}

TEST_CASE("Tests that iterators loop through all objects in octree") {
    Body obj(10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
    Body obj2(10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    Body obj3(10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    vector<Object> bodies({obj, obj2, obj3});
    Octree tree(bodies);
    size_t index = 0;
    for (auto& body : tree) {
        REQUIRE(body.mass == bodies[index].mass);
        REQUIRE(body.position == bodies[index++].position);
    }
}

TEST_CASE("Tests the equality operation on two octree iterators") {
    Body obj(10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
    Body obj2(10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    Body obj3(10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    vector<Object> bodies({obj, obj2, obj3});
    Octree tree(bodies);
    auto it = tree.begin();
    auto it2 = tree.begin();
    REQUIRE(it == it2);
    it2++;
    REQUIRE(it != it2);
    ++it;
    REQUIRE(it == it2);
}

// TEST_CASE(
//     "Tests that nodes of the tree can be traversed through root of octree") {
//     stack<OctreeNode*> s;
//     Body obj(10, Vec3{1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
//     Body obj2(10, Vec3{-1, -1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
//     Body obj3(10, Vec3{1, 1, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
//     vector<Object> bodies({obj, obj2, obj3});
//     Octree tree(bodies);
//     s.push(tree.root);
//     while (!s.empty()) {
//         auto current = s.top();
//         s.pop();
//         if (current->type == OctreeNodeType::EXTERNAL) {
//             if (!current->empty()) {
//                 ASSERT_FALSE(current->empty());
//                 ASSERT_EQUAL(current->getObject().mass, 10.0f);
//             } else {
//                 ASSERT_TRUE(current->empty());
//             }
//         } else {
//             ASSERT_NOT_EQUAL(current->getObject().mass, 10.0f);
//         }
//         for (size_t i = 0; i < 8; i++) {
//             if (current->children[i]) {
//                 s.push(current->children[i]);
//             }
//         }
//     }
// }

TEST_CASE("Tests that bounds adapt to width while building") {
    Body obj(10000, Vec3{1000, 0, 0}, Vec3{0, 1, 0}, Vec3{0, 0, 1});
    Body obj2(10000, Vec3{-1, -1000, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    Body obj3(10000, Vec3{1, 2005, -1}, Vec3{0, -1, 0}, Vec3{0, 0, -1});
    vector<Body> bodies({obj, obj2, obj3});
    Octree tree;
    for (Body& obj : bodies) {
        tree.insert(obj);
    }
    tree.buildTree();
}