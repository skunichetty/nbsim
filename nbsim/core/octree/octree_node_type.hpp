#pragma once
#ifndef OCTREE_NODE_TYPE_H
#define OCTREE_NODE_TYPE_H
// Storage for different types of octree nodes. Demarcation of internal and
// external nodes needs to be made explicit for implementation of the Barnes-Hut
// Algorithm
enum class OctreeNodeType : char { INTERNAL, EXTERNAL };
#endif