#include "nbsim/core/octree/body.hpp"

Body::Body() : Object{}, velocity{Vec3{0, 0, 0}}, acceleration{Vec3{0, 0, 0}} {}

Body::Body(double mass, Vec3 position, Vec3 velocity, Vec3 acceleration)
    : Object(mass, position), velocity{velocity}, acceleration{acceleration} {
  // perform bounds checking
  if (mass > 1e150) {
    throw std::runtime_error(
        "Error: Cannot create body with mass greater than 1e300 "
        "kilograms.");
  }
  if (std::abs(position.x) > 1e150 || std::abs(position.y) > 1e150 ||
      std::abs(position.z) > 1e150) {
    throw std::runtime_error(
        "Error: Cannot create body with position component farther than "
        "1e150 meters "
        "from origin.");
  }
  if (std::abs(velocity.x) > 1e50 || std::abs(velocity.y) > 1e50 ||
      std::abs(velocity.z) > 1e50) {
    throw std::runtime_error(
        "Error: Cannot create body with velocity component greater than "
        "1e150 meters per second.");
  }
  if (std::abs(acceleration.x) > 1e25 || std::abs(acceleration.y) > 1e25 ||
      std::abs(acceleration.z) > 1e25) {
    throw std::runtime_error(
        "Error: Cannot create body with acceleration component greater "
        "than "
        "1e150 meters per second.");
  }
}