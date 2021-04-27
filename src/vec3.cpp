#include "vec3.h"

std::ostream& operator<<(std::ostream& os, const Vec3& vec) {
    os << "[" << vec.x << "," << vec.y << "," << vec.z << "]";
    return os;
}