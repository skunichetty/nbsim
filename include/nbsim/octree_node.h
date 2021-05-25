#pragma once
#ifndef OCTREE_NODE_H
#define OCTREE_NODE_H

#include <vector>

#include "bounding_box.h"
#include "object.h"
#include "octant.h"
#include "octree_node_type.h"
#include "vec3.h"

/**
 * Node in an octree.
 */
class OctreeNode {
   private:
    // Gets the octant which this object should belong in with respect to this
    // region of space.
    Octant getOctant(Object* obj);
    // Gets center of mass of two objects
    Vec3 centerOfMass(Object* o1, Object* o2);
    // Inserts object into the correct octant of an octree
    void insertOctant(Object* obj);
    // Type of this node (internal or external)
    OctreeNodeType type;
    // Bounding region for this node
    BoundingBox box;
    // The "object" in this region. If an external
    // node, is a pointer to a Body.
    Object* localObj;

   public:
    // Children of this node
    std::vector<OctreeNode*> children;
    // Inserts the object into the subtree starting with this node
    void insert(Object* obj);
    // Returns if there is a local object here
    bool empty() const;
    // Returns read-only access to bounds of this node
    const BoundingBox& getBounds() const;
    // Returns type of this octree
    OctreeNodeType getType() const;
    // Returns read-only access to the local object stored here. If no object
    // stored, throws an error.
    const Object& getObject() const;
    // The Big Five
    OctreeNode(float width, Vec3& center);
    ~OctreeNode();
    OctreeNode(const OctreeNode& other);
    OctreeNode(OctreeNode&& other);
    OctreeNode& operator=(const OctreeNode& other);
    OctreeNode& operator=(OctreeNode&& other);
};

#endif