#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include <ostream>

#include "vec3.h"

/**
 * A simulation object - a body which interacts with others gravitationally
 */
struct Object {
    float mass;     // The mass of object - needed to compute force
    Vec3 position;  // The (x,y,z) position of object in space
    Vec3 velocity;  // The velocity of object - intermediary step of integration
    Vec3 acceleration;  // The acceleration of object - given by force
    friend std::ostream& operator<<(std::ostream& os, const Object& obj);
};

#endif