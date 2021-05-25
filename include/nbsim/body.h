#pragma once
#ifndef BODY_H
#define BODY_H

#include "object.h"

/**
 * An Object that experiences motion in the simulation. Is affected by gravity
 */
class Body : public Object {
   public:
    Vec3 velocity;
    Vec3 acceleration;
    Body();
    Body(float mass, Vec3 position, Vec3 velocity, Vec3 acceleration);
};

#endif