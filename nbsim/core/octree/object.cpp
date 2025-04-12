#include "nbsim/core/octree/object.hpp"

Object::Object() : mass{0}, position{Vec3{0, 0, 0}} {}

Object::Object(double mass, const Vec3& position) : mass{mass}, position{position} {}

bool Object::operator==(const Object& other) const { return (other.mass == mass) && (other.position == position); }

bool Object::operator!=(const Object& other) const { return !(other == *this); }
