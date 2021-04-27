#pragma once
#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <ostream>

/*
A 3D Vector to represent locations in simulation space
*/
struct Vec3 {
    double x;  // x component of vector
    double y;  // y component of vector
    double z;  // z component of vector

    // vector addition
    Vec3& operator+=(const Vec3& other) {
        if (this == &other) {
            *this = *this * 2;
            return *this;
        }
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    // reuse += operator b/c DRY code is good
    Vec3 operator+(const Vec3& other) {
        Vec3 copy = *this;
        copy += other;
        return copy;
    }

    // vector subtraction
    Vec3& operator-=(const Vec3& other) {
        if (this == &other) {
            *this = {0, 0, 0};
            return *this;
        }
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    // reuse -= operator b/c DRY code is good
    Vec3 operator-(const Vec3& other) {
        Vec3 copy = *this;
        copy -= other;
        return copy;
    }

    // scalar multiplication
    Vec3& operator*=(const float value) {
        x *= value;
        y *= value;
        z *= value;
        return *this;
    }

    // reuse -= operator b/c DRY code is good
    Vec3 operator*(const float value) {
        Vec3 copy = *this;
        copy *= value;
        return copy;
    }

    // scalar division
    Vec3& operator/=(const float value) {
        x /= value;
        y /= value;
        z /= value;
        return *this;
    }

    // reuse -= operator b/c DRY code is good
    Vec3 operator/(const float value) {
        Vec3 copy = *this;
        copy /= value;
        return copy;
    }

    // Returns the length of the vector
    double length() { return float(sqrt((x * x) + (y * y) + (z * z))); }

    // Write to output stream overload
    friend std::ostream& operator<<(std::ostream& os, const Vec3& vec);
};

#endif