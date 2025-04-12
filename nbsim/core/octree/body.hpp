#pragma once
#ifndef BODY_H
#define BODY_H

#include "nbsim/core/octree/object.hpp"

/**
 * An Object that experiences motion in the simulation. Is affected by gravity
 */
class Body : public Object {
public:
  Vec3 velocity;
  Vec3 acceleration;
  Body();
  Body(double mass, Vec3 position, Vec3 velocity, Vec3 acceleration);
};

#endif