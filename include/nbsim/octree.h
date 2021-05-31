#pragma once
#ifndef OCTREE_H
#define OCTREE_H

#include <algorithm>
#include <ostream>
#include <vector>

#include "body.h"
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
    // Body storage for octree. The tree nodes only contain
    // pointers, so only relative locations are maintained as
    // opposed to data types. Bodies stored separately to separate body access
    // from spatial hierarchy of tree.
    Body* bodies;
    // grows the internal object buffer
    void grow();

   public:
    // The root of the internal tree
    OctreeNode* root;
    // Adds body to tree.
    void insert(Body& body);
    // Prints a summary of all the current bodies and their state to the output
    // stream passed in
    void printSummary(std::ostream& os);
    // Builds the tree from root
    void buildTree();
    // Returns count of items stored
    size_t count() const;
    // Default constructor, with default simulation width of 1000 meters.
    Octree();
    // Constructor which takes in simWidth. If the maximum simulation width is
    // known beforehand, then tree construction is faster
    Octree(double simWidth);
    // Constructor with set of objects. If objects are preknown, tree
    // construction is faster.
    Octree(std::vector<Body>& inputBodies);

    /**
     * Allows iteration through objects stored in tree. Iteration is done in
     * order of insertion. C++ 17 style with tags.
     */
    class OctreeIterator {
        using Category = std::forward_iterator_tag;
        using DiffType = std::ptrdiff_t;
        using ValueType = Body;
        using Pointer = Body*;
        using Reference = Body&;

        Pointer obj_ptr;

       public:
        OctreeIterator(Pointer pointer) : obj_ptr(pointer) {}
        Reference operator*() const { return *obj_ptr; }
        bool operator==(const OctreeIterator& other) const {
            return obj_ptr == other.obj_ptr;
        }
        bool operator!=(const OctreeIterator& other) const {
            return !(*this == other);
        }
        // prefix
        OctreeIterator& operator++() {
            obj_ptr++;
            return *this;
        }
        // postfix
        OctreeIterator operator++(int) {
            OctreeIterator temp = *this;
            ++(*this);
            return temp;
        }
    };

    // Returns iterator to first object
    OctreeIterator begin();
    // Returns iterator to final object
    OctreeIterator end();

    // Big Five
    Octree& operator=(const Octree& other);
    Octree& operator=(Octree&& other);
    Octree(const Octree& other);
    Octree(Octree&& other);
    ~Octree();
};

#endif