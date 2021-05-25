#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include <ostream>

#include "vec3.h"

/**
 * A simulation object - a body which interacts with others gravitationally
 */
class Object {
   public:
    float mass;     // Mass of the object
    Vec3 position;  // Position of the object
    bool operator==(const Object& other) const;
    bool operator!=(const Object& other) const;
    Object();
    Object(float mass, const Vec3& position);
};

#endif