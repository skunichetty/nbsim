#pragma once
#ifndef OCTREE_H
#define OCTREE_H

#include <ostream>
#include <vector>

#include "object.h"
#include "octree_node.h"
#include "vec3.h"

/**
 * A tree with 8 children per node. This implementation is focused specifically
 * towards the application of Barnes-Hut algorithm.
 *
 * Width of the tree is adaptively determined, but if width is pre-emptively
 * known, it should be specified for faster performance. Additionally, if
 * objects to belong in tree are pre-emptively known, is faster to construct
 * tree with them.
 */
class Octree {
   private:
    // Allocated size of object buffer
    size_t allocSize;
    // Number of objects stored in object buffer
    size_t size;
    // Width of the root, a cuboid space
    double width;
    // The root of the internal tree
    OctreeNode* root;
    // The actual objects stored in this octree. This is a high
    // level storage of objects, and the tree nodes only contain
    // pointers. Thus only relative locations are maintained as
    // opposed to data types. Makes for easier tree
    // reconstruction as less memory is reallocated. (8 bytes vs 32 bytes)
    // To maintain permanance of pointers, we manually allocate space for
    // objects.
    Object* objects;
    // Gets approximate Euclidean distance between two points in space (omits
    // the square root for speed)
    double approx_distance(const Vec3& pos1, const Vec3& pos2) const;
    // Returns the gravitational force exerted between two objects
    Vec3 forceGravity(const Object* o1, const Object* o2) const;
    // Updates the forces between all different objects in the simulation
    void updateForces(double theta);
    // Updates the motion bewtween all different objects in the simulation
    void updateMotion(double dt);
    // grows the internal object buffer
    void grow();

   public:
    // Adds current object to tree.
    void insert(Object& object);
    // Traverses the tree and updates bodies
    void update(double theta, double dt);
    // Prints a summary of all the current bodies and their state to the output
    // stream passed in
    void printSummary(std::ostream& os);
    // Builds the tree from root
    void buildTree();

    // Default constructor, with default simulation width of 1000 meters.
    Octree();
    // Constructor which takes in simWidth. If the maximum simulation width is
    // known beforehand, then tree construction is faster
    Octree(double simWidth);
    // Constructor with set of objects. If objects are preknown, tree
    // construction is faster.
    Octree(std::vector<Object>& objects);

    // Big Five
    Octree& operator=(const Octree& other);
    Octree& operator=(Octree&& other);
    Octree(const Octree& other);
    Octree(Octree&& other);
    ~Octree();
};

#endif