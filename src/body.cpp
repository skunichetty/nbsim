#include "body.h"

Body::Body() : Object{}, velocity{Vec3{0, 0, 0}}, acceleration{Vec3{0, 0, 0}} {}

Body::Body(float mass, Vec3 position, Vec3 velocity, Vec3 acceleration)
    : Object(mass, position), velocity{velocity}, acceleration{acceleration} {}