#include "nbsim/core/vec3/vec3.hpp"

std::ostream &operator<<(std::ostream &os, const Vec3 &vec) {
  os << "[" << vec.x << "," << vec.y << "," << vec.z << "]";
  return os;
}