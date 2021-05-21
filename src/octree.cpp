#include "octree.h"

#include <iostream>
#include <stack>

using namespace std;

Octree::~Octree() {
    delete root;
    delete[] objects;
}

Octree::Octree()
    : allocSize{8},
      size{0},
      width{1000},
      root{nullptr},
      objects{new Object[allocSize]} {}

Octree::Octree(double simWidth)
    : allocSize{8},
      size{0},
      width{simWidth},
      root{nullptr},
      objects{new Object[allocSize]} {}

Octree::Octree(vector<Object>& inputObjects)
    : allocSize{inputObjects.size()},
      size{0},
      root{nullptr},
      objects{new Object[allocSize]} {
    // find max location of all objects
    double max_coord = 0.0;
    for (const Object& object : inputObjects) {
        double max_obj_coord =
            max(object.position.x, max(object.position.y, object.position.z));
        max_coord = max(max_coord, max_obj_coord);
        objects[size++] = object;
    }
    width = max_coord;
    buildTree();
}

Octree::Octree(const Octree& other)
    : allocSize{other.allocSize},
      size{other.size},
      width{other.width},
      root{nullptr},
      objects{new Object[allocSize]} {
    root = new OctreeNode(*other.root);
    for (Object* ptr = other.objects; size_t(ptr - other.objects) < other.size;
         ++ptr) {
        *(objects + (ptr - other.objects)) = *ptr;
    }
}

Octree::Octree(Octree&& other)
    : allocSize{other.allocSize},
      size{other.size},
      width{other.width},
      root{other.root},
      objects{other.objects} {
    other.root = nullptr;
    other.objects = nullptr;
}

Octree& Octree::operator=(const Octree& other) { return *this = Octree(other); }

Octree& Octree::operator=(Octree&& other) {
    swap(allocSize, other.allocSize);
    swap(size, other.size);
    swap(width, other.width);
    swap(root, other.root);
    swap(objects, other.objects);
    return *this;
}

void Octree::grow() {
    // double the internal buffer of octree
    allocSize *= 2;
    Object* temp = new Object[allocSize];
    // copy over all objects
    for (Object* ptr = objects; ptr; ptr++) {
        *(temp + (ptr - objects)) = *ptr;
    }
    delete[] objects;
    objects = temp;
    // as memory locations have changed, rebuild the tree
    buildTree();
}

void Octree::insert(Object& object) {
    // if object not in current bounds, expand width to fit it.
    if (object.position.x > width / 2 || object.position.x < -1 * width / 2 ||
        object.position.y > width / 2 || object.position.y < -1 * width / 2 ||
        object.position.z > width / 2 || object.position.z < -1 * width / 2) {
        // increase by 1.5 to anticipate further regrowths
        width = 1.5 * max(abs(object.position.x),
                          max(abs(object.position.y), abs(object.position.z)));
        buildTree();
    }
    // if not enough space in array
    if (allocSize == size) grow();
    objects[size++] = object;
    if (!root) {
        Vec3 center = {0, 0, 0};
        root = new OctreeNode(width, center);
    }
    root->insert(&object);
}

double Octree::approx_distance(const Vec3& pos1, const Vec3& pos2) const {
    double delta_x = (pos1.x - pos2.x);
    double delta_y = (pos1.y - pos2.y);
    double delta_z = (pos1.z - pos2.z);
    return (delta_x * delta_x) + (delta_y * delta_y) + (delta_z * delta_z);
}

Vec3 Octree::forceGravity(const Object* o1, const Object* o2) const {
    Vec3 r12 = o2->position - o1->position;
    double G = 6.678E-11;
    double constantTerm =
        G * o1->mass * o2->mass / (r12.length() * r12.length() * r12.length());
    Vec3 force = r12 * constantTerm;
    return force;
}

void Octree::updateForces(double theta) {
    // this is a modified DFS which only traverses all nodes for which the
    // region they encompass is much larger than the distance between this
    // region center of mass and the other bodym
    for (size_t i = 0; i < size; i++) {
        // calculate net force for each object
        stack<OctreeNode*> searchContainer;
        Vec3 netForce{0, 0, 0};
        searchContainer.push(root);
        while (!searchContainer.empty()) {
            OctreeNode* current = searchContainer.top();
            searchContainer.pop();
            auto d = approx_distance(objects[i].position,
                                     current->localObj->position);
            if (current->localObj != &objects[i] &&
                (current->box.width * current->box.width) / d > theta) {
                // then the region is too large - need to traverse children
                for (auto ptr : current->children) {
                    if (ptr != nullptr && ptr->localObj != &objects[i]) {
                        // only push nodes which are occupied and not the same
                        // as the current one being considered
                        searchContainer.push(ptr);
                    }
                }
            } else {
                // update the force
                if (current != nullptr && current->localObj != nullptr &&
                    current->localObj != &objects[i]) {
                    netForce += forceGravity(&objects[i], current->localObj);
                }
            }
        }
        objects[i].acceleration += netForce / objects[i].mass;
    }
}

void Octree::updateMotion(double dt) {
    // Integrate acceleration into velocity, and velocity into position
    for (size_t i = 0; i < size; i++) {
        objects[i].velocity += objects[i].acceleration * dt;
        objects[i].position += objects[i].velocity * dt;
    }
}

void Octree::update(double theta, double dt) {
    // Step 1 - compute all forces on each object
    updateForces(theta);
    // Step 2 - update the motion for each object
    updateMotion(dt);
}

void Octree::printSummary(ostream& os) {
    os << "=======SUMMARY======="
       << "\n";
    for (size_t i = 0; i < size; i++) {
        os << objects[i] << '\n';
    }
}

void Octree::buildTree() {
    delete root;
    Vec3 center = {0, 0, 0};
    root = new OctreeNode(width, center);
    for (size_t i = 0; i < size; i++) {
        root->insert(&objects[i]);
    }
}