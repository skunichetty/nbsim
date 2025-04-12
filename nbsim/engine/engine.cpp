#include "nbsim/engine/engine.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stack>

using namespace std;

Engine::Engine(double theta, double dt) : currentTime{0.0}, theta{theta}, dt{dt}, tree{} {}

Engine::Engine(double theta, double dt, double simulationWidth)
    : currentTime{0.0},
      theta{theta},
      dt{dt},
      tree{Octree(simulationWidth)} {}

Engine::Engine(double theta, double dt, std::vector<Body>& bodies)
    : currentTime{0.0},
      theta{theta},
      dt{dt},
      tree{Octree(bodies)} {}

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

void Engine::computeForce(OctreeNode* root, Body& body) {
    if (root) {
        if (root->getType() != OctreeNodeType::EXTERNAL) {
            const BoundingBox bounds = root->getBounds();
            auto d = approx_distance(root->getObject().position, body.position);
            if ((bounds.width * bounds.width) / d > (theta * theta)) {
                for (size_t i = 0; i < 8; i++) {
                    computeForce(root->children[i], body);
                }
            }
        } else if (&root->getObject() != &body) {
            body.acceleration += accelerationGravity(root->getObject(), body);
        }
    }
}

void Engine::updateForces(double theta) {
    if (tree.root->empty())
        return;
    for (auto& object : tree) {
        computeForce(tree.root, object);
    }
}

void Engine::updateMotion(double dt) {
    // Integrate acceleration into velocity, and velocity into position
    for (auto& object : tree) {
        object.velocity += object.acceleration * dt;
        object.position += object.velocity * dt;
    }
}

inline Vec3 Engine::accelerationGravity(const Object& o1, const Object& o2) const {
    Vec3 r12 = o2.position - o1.position;
    double G = 6.678E-11;
    double constantTerm = -1 * G * o1.mass / (r12.length() * r12.length() * r12.length());
    Vec3 force = r12 * constantTerm;
    return force;
}

string Engine::printStateJson() {
    ostringstream stringBuilder;
    stringBuilder << setprecision(5); // set decimal precision to 5 pts
    stringBuilder << "{\"time\":" << currentTime << ",";
    stringBuilder << "\"bodies\":[";
    size_t index = 0;
    for (auto& object : tree) {
        stringBuilder << "{\"mass\":" << object.mass;
        stringBuilder << ",\"position\":{"
                      << "\"x\":" << object.position.x << ",\"y\":" << object.position.y
                      << ",\"z\":" << object.position.z << "}";
        stringBuilder << ",\"velocity\":{"
                      << "\"x\":" << object.velocity.x << ",\"y\":" << object.velocity.y
                      << ",\"z\":" << object.velocity.z << "}";
        stringBuilder << ",\"acceleration\":{"
                      << "\"x\":" << object.acceleration.x << ",\"y\":" << object.acceleration.y
                      << ",\"z\":" << object.acceleration.z << "}";
        stringBuilder << "}";
        if (index < tree.count() - 1)
            stringBuilder << ",";
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