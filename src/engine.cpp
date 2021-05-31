#include "engine.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stack>

using namespace std;

Engine::Engine(double theta = 0.5, double dt = 0.1)
    : currentTime{0.0}, theta{theta}, dt{dt}, tree{} {}

Engine::Engine(double theta, double dt, double simulationWidth)
    : currentTime{0.0}, theta{theta}, dt{dt}, tree{Octree(simulationWidth)} {}

Engine::Engine(double theta, double dt, std::vector<Body>& bodies)
    : currentTime{0.0}, theta{theta}, dt{dt}, tree{Octree(bodies)} {}

void Engine::addBody(Body& body) { tree.insert(body); }

string Engine::step() {
    // Step 1 - compute all forces on each object
    updateForces(theta);
    // Step 2 - update the motion for each object
    updateMotion(dt);
    currentTime += dt;
    tree.buildTree();
    return printStateJson();
}

void Engine::updateForces(double theta) {
    // this is a modified DFS which only traverses all nodes for which the
    // region they encompass is much larger than the distance between this
    // region center of mass and the other body
    if (tree.root->empty()) return;
    for (auto& object : tree) {
        // calculate net force for each object
        Vec3 netForce{0, 0, 0};
        stack<OctreeNode*> searchContainer;
        searchContainer.push(tree.root);
        while (!searchContainer.empty()) {
            OctreeNode* current = searchContainer.top();
            searchContainer.pop();
            // Checks if node is valid, is nonempty, and does not contain the
            // same object
            if (current && !current->empty() &&
                &current->getObject() != &object) {
                const Object& obj = current->getObject();
                if (current->getType() == OctreeNodeType::EXTERNAL) {
                    netForce += forceGravity(object, obj);
                } else {
                    const BoundingBox bounds = current->getBounds();
                    auto d = approx_distance(object.position, obj.position);
                    if ((bounds.width * bounds.width) / d > theta) {
                        // then the region is too large - need to traverse
                        // children
                        for (auto ptr : current->children) {
                            if (ptr != nullptr) {
                                // only push nodes which are occupied and not
                                // the same as the current one being considered
                                searchContainer.push(ptr);
                            }
                        }
                    } else {
                        netForce += forceGravity(object, obj);
                    }
                }
            }
        }
        object.acceleration += netForce / object.mass;
    }
}

void Engine::updateMotion(double dt) {
    // Integrate acceleration into velocity, and velocity into position
    for (auto& object : tree) {
        object.velocity += object.acceleration * dt;
        object.position += object.velocity * dt;
    }
}

Vec3 Engine::forceGravity(const Object& o1, const Object& o2) const {
    Vec3 r12 = o2.position - o1.position;
    double G = 6.678E-11;
    double constantTerm =
        G * o1.mass * o2.mass / (r12.length() * r12.length() * r12.length());
    Vec3 force = r12 * constantTerm;
    return force;
}

string Engine::printStateJson() {
    ostringstream stringBuilder;
    stringBuilder << setprecision(5);  // set decimal precision to 5 pts
    stringBuilder << "{\"time\":" << currentTime << ",";
    stringBuilder << "\"bodies\":[";
    size_t index = 0;
    for (auto& object : tree) {
        stringBuilder << "{\"mass\":" << object.mass;
        stringBuilder << ",\"position\":{"
                      << "\"x\":" << object.position.x
                      << ",\"y\":" << object.position.y
                      << ",\"z\":" << object.position.z << "}";
        stringBuilder << ",\"velocity\":{"
                      << "\"x\":" << object.velocity.x
                      << ",\"y\":" << object.velocity.y
                      << ",\"z\":" << object.velocity.z << "}";
        stringBuilder << ",\"acceleration\":{"
                      << "\"x\":" << object.acceleration.x
                      << ",\"y\":" << object.acceleration.y
                      << ",\"z\":" << object.acceleration.z << "}";
        stringBuilder << "}";
        if (index < tree.count() - 1) stringBuilder << ",";
        index++;
    }
    stringBuilder << "]}";
    return stringBuilder.str();
}

double Engine::approx_distance(const Vec3& pos1, const Vec3& pos2) const {
    double dx = (pos1.x - pos2.x);
    double dy = (pos1.y - pos2.y);
    double dz = (pos1.z - pos2.z);
    return dx * dx + dy * dy + dz * dz;
}