#pragma once
#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "vec3.h"

/**
 * A cuboid bounding box
 */
struct BoundingBox {
    Vec3 center;   // Center of this bounding box
    double width;  // Width from one side to another
    BoundingBox(double width) : center{Vec3{0, 0, 0}}, width{width} {}
    BoundingBox(double x, double y, double z, double width)
        : center{Vec3{x, y, z}}, width{width} {}
    BoundingBox(const Vec3& center, double width)
        : center{center}, width{width} {}
};

#endif