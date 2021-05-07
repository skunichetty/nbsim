#include "octree_node.h"

using namespace std;

// default constructor
OctreeNode::OctreeNode(double width, Vec3& center)
    : type{OctreeNodeType::EXTERNAL},
      width{width},
      localObj{nullptr},
      center{center},
      children{vector<OctreeNode*>(8, nullptr)} {
    // every single node starts off as external - only by growing does it become
    // internal
}

// copy constructor
OctreeNode::OctreeNode(const OctreeNode& other)
    : type{other.type},
      width{other.width},
      localObj{nullptr},
      center{other.center},
      children{vector<OctreeNode*>(8, nullptr)} {
    if (type == OctreeNodeType::EXTERNAL) {
        localObj = other.localObj;
    } else {
        localObj =
            new Object{other.localObj->mass, other.localObj->position,
                       other.localObj->velocity, other.localObj->acceleration};
        for (size_t i = 0; i < other.children.size(); i++) {
            if (other.children[i]) {
                children[i] = new OctreeNode(*other.children[i]);
            }
        }
    }
}

// assignment operator. Uses copy swap idiom.
OctreeNode& OctreeNode::operator=(const OctreeNode& other) {
    return *this = OctreeNode(other);
}

// move constructor
OctreeNode::OctreeNode(OctreeNode&& other)
    : type{other.type},
      width{other.width},
      localObj{other.localObj},
      center{other.center},
      children{other.children} {
    other.localObj = nullptr;
    other.children = vector<OctreeNode*>(8, nullptr);
}

// move assignment operator
OctreeNode& OctreeNode::operator=(OctreeNode&& other) {
    std::swap(other.type, type);
    std::swap(other.width, width);
    std::swap(other.localObj, localObj);
    std::swap(other.center, center);
    std::swap(other.children, children);
    return *this;
}

OctreeNode::~OctreeNode() {
    if (type == OctreeNodeType::INTERNAL) {
        // if an internal node, localObj is phantom obj - this instance owns it
        delete localObj;
        // clear all children nodes and reset them
        for (auto& child : children) {
            delete child;
            child = nullptr;
        }
    }
    // if an external node, this instance does not own the memory pointed to by
    // localObj, so we don't delete it
}

void OctreeNode::insert_helper(Object* obj) {
    // update center of mass info
    localObj->position = centerOfMass(obj, localObj);
    localObj->mass += obj->mass;
    // insert child in its respective octant
    Octant oct = getOctant(obj);
    int index = static_cast<int>(oct);
    if (!(children[index])) {
        // a node doesn't exist here - init it
        switch (oct) {
            case Octant::FIRST: {
                Vec3 newCenter =
                    center + Vec3{width / 4.0, width / 4.0, width / 4.0};
                children[index] = new OctreeNode(width / 2, newCenter);
                break;
            }
            case Octant::SECOND: {
                Vec3 newCenter =
                    center + Vec3{width / 4.0, width / 4.0, -1 * width / 4.0};
                children[index] = new OctreeNode(width / 2, newCenter);
                break;
            }
            case Octant::THIRD: {
                Vec3 newCenter =
                    center + Vec3{width / 4.0, -1 * width / 4.0, width / 4.0};
                children[index] = new OctreeNode(width / 2, newCenter);
                break;
            }
            case Octant::FOURTH: {
                Vec3 newCenter = center + Vec3{width / 4.0, -1 * width / 4.0,
                                               -1 * width / 4.0};
                children[index] = new OctreeNode(width / 2, newCenter);
                break;
            }
            case Octant::FIFTH: {
                Vec3 newCenter =
                    center + Vec3{-1 * width / 4.0, width / 4.0, width / 4.0};
                children[index] = new OctreeNode(width / 2, newCenter);
                break;
            }
            case Octant::SIXTH: {
                Vec3 newCenter = center + Vec3{-1 * width / 4.0, width / 4.0,
                                               -1 * width / 4.0};
                children[index] = new OctreeNode(width / 2, newCenter);
                break;
            }
            case Octant::SEVENTH: {
                Vec3 newCenter = center + Vec3{-1 * width / 4.0,
                                               -1 * width / 4.0, width / 4.0};
                children[index] = new OctreeNode(width / 2, newCenter);
                break;
            }
            case Octant::EIGHT: {
                Vec3 newCenter =
                    center +
                    Vec3{-1 * width / 4.0, -1 * width / 4.0, -1 * width / 4.0};
                children[index] = new OctreeNode(width / 2, newCenter);
                break;
            }
        }
    }
    children[index]->insert(obj);
}

void OctreeNode::insert(Object* obj) {
    if (type == OctreeNodeType::EXTERNAL && localObj == nullptr) {
        // empty external node - just place body here
        localObj = obj;
    } else {
        if (type == OctreeNodeType::EXTERNAL) {
            // convert from external to internal
            Object* temp = localObj;
            localObj = nullptr;
            localObj =
                new Object{0, Vec3{0, 0, 0}, Vec3{0, 0, 0}, Vec3{0, 0, 0}};
            // insert the old object in new octant
            insert_helper(temp);
            type = OctreeNodeType::INTERNAL;
        }
        insert_helper(obj);
    }
}

Octant OctreeNode::getOctant(Object* obj) {
    if (obj->position.x < center.x && obj->position.y < center.y &&
        obj->position.z < center.z) {
        // is in octant 8
        return Octant::EIGHT;
    } else if (obj->position.x < center.x && obj->position.y < center.y &&
               obj->position.z >= center.z) {
        // is in octant 7
        return Octant::SEVENTH;
    } else if (obj->position.x < center.x && obj->position.y >= center.y &&
               obj->position.z < center.z) {
        // is in octant 6
        return Octant::SIXTH;
    } else if (obj->position.x < center.x && obj->position.y >= center.y &&
               obj->position.z >= center.z) {
        // is in octant 5
        return Octant::FIFTH;
    } else if (obj->position.x >= center.x && obj->position.y < center.y &&
               obj->position.z < center.z) {
        // is in octant 4
        return Octant::FOURTH;
    } else if (obj->position.x >= center.x && obj->position.y < center.y &&
               obj->position.z >= center.z) {
        // is in octant 3
        return Octant::THIRD;
    } else if (obj->position.x >= center.x && obj->position.y >= center.y &&
               obj->position.z < center.z) {
        // is in octant 2
        return Octant::SECOND;
    }
    // is in octant 1
    return Octant::FIRST;
}

Vec3 OctreeNode::centerOfMass(Object* o1, Object* o2) {
    Vec3 temp = o1->position * o1->mass + o2->position * o2->mass;
    temp /= (o1->mass + o2->mass);
    return temp;
}