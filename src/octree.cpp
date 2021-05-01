#include "octree.h"

#include <iostream>
#include <stack>

using namespace std;

Octree::~Octree() { delete root; }

Octree::Octree() : simWidth{1000}, root{nullptr}, objects{vector<Object>(0)} {}

Octree::Octree(double simWidth)
    : simWidth{simWidth}, root{nullptr}, objects{vector<Object>(0)} {}

Octree::Octree(const Octree& other)
    : simWidth{other.simWidth}, root{nullptr}, objects{other.objects} {
    root = new OctreeNode(*other.root);
}

Octree& Octree::operator=(const Octree& other) {
    Octree temp = *this;
    swap(temp, *this);
    return *this;
}

void Octree::insert(Object& object) {
    objects.push_back(object);
    if (!root) {
        Vec3 center = {0, 0, 0};
        root = new OctreeNode(OctreeNodeType::EXTERNAL, simWidth, center);
    }
    root->insert(&object);
}

double Octree::approx_distance(const Vec3& pos1, const Vec3& pos2) const {
    double delta_x = (pos1.x - pos2.x);
    double delta_y = (pos1.y - pos2.y);
    double delta_z = (pos1.z - pos2.z);
    return (delta_x * delta_x) + (delta_y * delta_y) + (delta_z * delta_z);
}

double Octree::distance(const Vec3& pos1, const Vec3& pos2) const {
    return sqrt(approx_distance(pos1, pos2));
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
    // region center of mass and the other body
    for (auto& object : objects) {
        // calculate net force for each object
        stack<OctreeNode*> searchContainer;
        Vec3 netForce{0, 0, 0};
        searchContainer.push(root);
        while (!searchContainer.empty()) {
            OctreeNode* current = searchContainer.top();
            searchContainer.pop();
            auto d =
                approx_distance(object.position, current->localObj->position);
            if (current->localObj != &object &&
                (current->width * current->width) / d > theta) {
                // then the region is too large - need to traverse children
                for (auto ptr : current->children) {
                    if (ptr != nullptr && ptr->localObj != &object) {
                        // only push nodes which are occupied and not the same
                        // as the current one being considered
                        searchContainer.push(ptr);
                    }
                }
            } else {
                // update the force
                if (current != nullptr && current->localObj != nullptr &&
                    current->localObj != &object) {
                    netForce += forceGravity(&object, current->localObj);
                }
            }
        }
        object.acceleration += netForce / object.mass;
    }
}

void Octree::updateMotion(double dt) {
    // Integrate acceleration into velocity, and velocity into position
    for (auto& object : objects) {
        object.velocity += object.acceleration * dt;
        object.position += object.velocity * dt;
    }
}

void Octree::update(double theta, double dt) {
    // Step 1 - compute all forces on each object
    updateForces(theta);
    // Step 2 - update the motion for each object
    updateMotion(dt);
}

void Octree::printSummary() {
    std::cout << "=======SUMMARY======="
              << "\n";
    for (auto& object : objects) {
        std::cout << object << "\n";
    }
}

void Octree::rebuildTree() {
    delete root;
    Vec3 center = {0, 0, 0};
    root = new OctreeNode(OctreeNodeType::EXTERNAL, simWidth, center);
    for (auto& object : objects) {
        root->insert(&object);
    }
}