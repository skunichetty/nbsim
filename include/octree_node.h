#pragma once
#ifndef OCTREE_NODE_H
#define OCTREE_NODE_H

#include <vector>

#include "object.h"
#include "octant.h"
#include "octree_node_type.h"
#include "vec3.h"

/**
 * Node in an octree.
 */
class OctreeNode {
   private:
    // Type of this node (internal or external)
    OctreeNodeType type;
    // Width of region (meters)
    double width;
    // The "object" in this region. If an internal node, is a phantom object
    // representing the center of mass of this region of space. If an external
    // node, is an actual object in the simulation
    Object* localObj;
    // The center point of this region of space.
    Vec3 center;
    // Children of this node
    std::vector<OctreeNode*> children;
    /**
     * Gets the octant which this object should belong in with respect to this
     * region of space.
     */
    Octant getOctant(Object* obj);
    // gets center of mass of two objects
    Vec3 centerOfMass(Object* o1, Object* o2);
    // Handles the insertion of node into child substructure when internal.
    // Abstracts away node initialization details
    void insert_helper(Object* obj);

   public:
    void insert(Object* obj);
    OctreeNode(OctreeNodeType type, double width, Vec3& center);
    ~OctreeNode();
    OctreeNode(const OctreeNode& other);
    OctreeNode& operator=(const OctreeNode& other);
};

#endif