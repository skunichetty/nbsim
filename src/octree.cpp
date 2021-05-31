#include "octree.h"

#include <iostream>
#include <stack>

using namespace std;

Octree::~Octree() {
    delete root;
    delete[] bodies;
}

Octree::Octree()
    : allocSize{8},
      size{0},
      width{1000},
      bodies{new Body[allocSize]},
      root{nullptr} {}

Octree::Octree(double simWidth)
    : allocSize{8},
      size{0},
      width{simWidth},
      bodies{new Body[allocSize]},
      root{nullptr} {}

Octree::Octree(vector<Body>& inputBodies)
    : allocSize{inputBodies.size()},
      size{0},
      bodies{new Body[allocSize]},
      root{nullptr} {
    // find max location of all objects
    double max_coord = 0.0;
    for (const Body& object : inputBodies) {
        double max_obj_coord =
            max(abs(object.position.x),
                max(abs(object.position.y), abs(object.position.z)));
        max_coord = max(max_coord, max_obj_coord);
        bodies[size++] = object;
    }
    width = 2 * max_coord;
    buildTree();
}

Octree::Octree(const Octree& other)
    : allocSize{other.allocSize},
      size{other.size},
      width{other.width},
      bodies{new Body[allocSize]},
      root{nullptr} {
    root = new OctreeNode(*other.root);
    for (Body* ptr = other.bodies; size_t(ptr - other.bodies) < other.size;
         ++ptr) {
        *(bodies + (ptr - other.bodies)) = *ptr;
    }
}

Octree::Octree(Octree&& other)
    : allocSize{other.allocSize},
      size{other.size},
      width{other.width},
      bodies{other.bodies},
      root{other.root} {
    other.root = nullptr;
    other.bodies = nullptr;
}

Octree& Octree::operator=(const Octree& other) { return *this = Octree(other); }

Octree& Octree::operator=(Octree&& other) {
    swap(allocSize, other.allocSize);
    swap(size, other.size);
    swap(width, other.width);
    swap(bodies, other.bodies);
    return *this;
}

void Octree::grow() {
    // double the internal buffer of octree
    allocSize *= 2;
    Body* temp = new Body[allocSize];
    // copy over all objects
    for (Body* ptr = bodies; size_t(ptr - bodies) < size; ptr++) {
        *(temp + (ptr - bodies)) = *ptr;
    }
    delete[] bodies;
    bodies = temp;
    // as memory locations have changed, rebuild the tree
    buildTree();
}

void Octree::insert(Body& body) {
    // if object not in current bounds, expand width to fit it.
    if (body.position.x > width / 2 || body.position.x < -1 * width / 2 ||
        body.position.y > width / 2 || body.position.y < -1 * width / 2 ||
        body.position.z > width / 2 || body.position.z < -1 * width / 2) {
        // increase by 1.5 to anticipate further regrowths
        width = 3 * max(abs(body.position.x),
                        max(abs(body.position.y), abs(body.position.z)));
        buildTree();
    }
    // if not enough space in array
    if (allocSize == size) grow();
    bodies[size++] = body;
    if (!root) {
        Vec3 center = {0, 0, 0};
        root = new OctreeNode(width, center);
    }
    root->insert(&bodies[size - 1]);
}

void Octree::printSummary(ostream& os) {
    os << "=======SUMMARY======="
       << "\n";
    for (size_t i = 0; i < size; i++) {
        os << "( " << bodies[i].mass << "," << bodies[i].position << ")\n";
    }
}

void Octree::buildTree() {
    delete root;
    Vec3 center = {0, 0, 0};
    root = new OctreeNode(width, center);
    for (size_t i = 0; i < size; i++) {
        root->insert(&bodies[i]);
    }
}

Octree::OctreeIterator Octree::begin() { return OctreeIterator(&bodies[0]); }
Octree::OctreeIterator Octree::end() { return OctreeIterator(&bodies[size]); }

size_t Octree::count() const { return size; }