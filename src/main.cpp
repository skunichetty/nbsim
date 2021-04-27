#include <iostream>

#include "object.h"
#include "octree.h"
#include "vec3.h"

using namespace std;

int main() {
    vector<Object> objects = {
        Object{10, Vec3{0, 10, 0}, Vec3{0, 0, 0}, Vec3{0, 0, 0}},
        Object{10, Vec3{0, -1, 0}, Vec3{0, 0, 0}, Vec3{0, 0, 0}},
        Object{10, Vec3{0, 0, 0}, Vec3{0, 0, 0}, Vec3{0, 0, 0}},
        Object{10, Vec3{-1, 10, 1}, Vec3{0, 0, 0}, Vec3{0, 0, 0}},
        Object{10, Vec3{0, 10, -5.6}, Vec3{0, 0, 0}, Vec3{0, 0, 0}},
        Object{10, Vec3{0, 3, 2}, Vec3{0, 0, 0}, Vec3{0, 0, 0}},
        Object{10, Vec3{3, 0, 0}, Vec3{0, 0, 0}, Vec3{0, 0, 0}},
        Object{10, Vec3{-5, 10, 0}, Vec3{0, 0, 0}, Vec3{0, 0, 0}},
        Object{10, Vec3{5, 10, -2}, Vec3{0, 0, 0}, Vec3{0, 0, 0}},
        Object{10, Vec3{0, 10, 1}, Vec3{0, 0, 0}, Vec3{0, 0, 0}},
        Object{10, Vec3{2, 10, 0}, Vec3{0, 0, 0}, Vec3{0, 0, 0}},
    };
    Octree octree(100, objects);
    cout << "Finished" << endl;
}