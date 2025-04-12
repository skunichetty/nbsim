#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include <ostream>

#include "nbsim/core/vec3/vec3.hpp"

/**
 * A simulation object - a body which interacts with others gravitationally
 */
class Object {
public:
  double mass;   // Mass of the object
  Vec3 position; // Position of the object
  bool operator==(const Object &other) const;
  bool operator!=(const Object &other) const;
  Object();
  Object(double mass, const Vec3 &position);
};

#endif