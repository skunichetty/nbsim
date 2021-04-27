#pragma once
#ifndef OCTREE_H
#define OCTREE_H

#include <vector>

#include "object.h"
#include "octree_node.h"
#include "vec3.h"

/**
 * A tree with 8 children. This implementation is focused specifically towards
 * the application of Barnes-Hut algorithm
 */
class Octree {
   private:
    // Width of the simulation space, a cuboid space
    double simWidth;
    OctreeNode* root;
    // The actual objects stored in this octree. This is a high
    // level storage of objects, and the tree nodes only contain
    // pointers. Thus only relative locations are maintained as
    // opposed to data types. Makes for easier tree
    // reconstruction.
    std::vector<Object>& objects;
    // Builds the tree with the current set of objects
    void buildTree();

   public:
    void reconstruct();
    Octree(double simWidth, std::vector<Object>& objects);
    ~Octree();
};

#endif