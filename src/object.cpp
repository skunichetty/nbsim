#include "object.h"

std::ostream& operator<<(std::ostream& os, const Object& obj) {
    os << "{" << obj.mass << ", " << obj.position << "}";
    return os;
}