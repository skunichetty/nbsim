#pragma once
#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "vec3.h"

/**
 * A cuboid bounding box
 */
struct BoundingBox {
    Vec3 center;  // Center of this bounding box
    float width;  // Width from one side to another
    BoundingBox(float width) : center{Vec3{0, 0, 0}}, width{width} {}
    BoundingBox(float x, float y, float z, float width)
        : center{Vec3{x, y, z}}, width{width} {}
    BoundingBox(const Vec3& center, float width)
        : center{center}, width{width} {}
};

#endif