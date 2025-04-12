#pragma once
#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "nbsim/core/vec3/vec3.hpp"

/**
 * A cuboid bounding box
 */
struct BoundingBox {
  double width; // Width from one side to another
  Vec3 center;  // Center of this bounding box
  BoundingBox(double width) : width{width}, center{Vec3{0, 0, 0}} {}
  BoundingBox(double x, double y, double z, double width)
      : width{width}, center{Vec3{x, y, z}} {}
  BoundingBox(const Vec3 &center, double width)
      : width{width}, center{center} {}
};

#endif