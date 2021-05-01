#pragma once
#ifndef OCTREE_H
#define OCTREE_H

#include <vector>

#include "object.h"
#include "octree_node.h"
#include "vec3.h"

/**
 * A tree with 8 children per node. This implementation is focused specifically
 * towards the application of Barnes-Hut algorithm
 */
class Octree {
   private:
    // Width of the root, a cuboid space
    double simWidth;
    OctreeNode* root;
    // The actual objects stored in this octree. This is a high
    // level storage of objects, and the tree nodes only contain
    // pointers. Thus only relative locations are maintained as
    // opposed to data types. Makes for easier tree
    // reconstruction.
    std::vector<Object> objects;
    // Gets Euclidean distance between two points in space
    double distance(const Vec3& pos1, const Vec3& pos2) const;
    // Gets approximate Euclidean distance between two points in space (omits
    // the square root for speed)
    double approx_distance(const Vec3& pos1, const Vec3& pos2) const;
    // Returns the gravitational force exerted between two objects
    Vec3 forceGravity(const Object* o1, const Object* o2) const;
    // Updates the forces between all different objects in the simulation
    void updateForces(double theta);
    // Updates the motion bewtween all different objects in the simulation
    void updateMotion(double dt);

   public:
    // Adds current object to tree.
    void insert(Object& object);
    // Traverses the tree and updates bodies
    void update(double theta, double dt);
    // prints a summary of all the current bodies and their state
    void printSummary();
    // rebuilds the tree from root
    void rebuildTree();
    // Assignment operator
    Octree& operator=(const Octree& other);
    Octree();
    Octree(double simWidth);
    Octree(const Octree& other);
    ~Octree();
};

#endif