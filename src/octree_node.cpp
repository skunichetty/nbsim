#include "octree_node.h"

using namespace std;

// default constructor
OctreeNode::OctreeNode(double width, Vec3& center)
    : type{OctreeNodeType::EXTERNAL},
      box{BoundingBox(center, width)},
      localObj{nullptr},
      children{vector<OctreeNode*>(8, nullptr)} {
    // every single node starts off as external - only by growing does it become
    // internal
}

// copy constructor
OctreeNode::OctreeNode(const OctreeNode& other)
    : type{other.type},
      box{BoundingBox(other.box.center, other.box.width)},
      localObj{nullptr},
      children{vector<OctreeNode*>(8, nullptr)} {
    if (type == OctreeNodeType::EXTERNAL) {
        localObj = other.localObj;
    } else {
        localObj = new Object(other.localObj->mass, other.localObj->position);
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
      box{BoundingBox(other.box.center, other.box.width)},
      localObj{other.localObj},
      children{other.children} {
    other.localObj = nullptr;
    other.children = vector<OctreeNode*>(8, nullptr);
}

// move assignment operator
OctreeNode& OctreeNode::operator=(OctreeNode&& other) {
    swap(other.type, type);
    swap(other.box, box);
    swap(other.localObj, localObj);
    swap(other.children, children);
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

void OctreeNode::insertOctant(Object* obj) {
    // update center of mass info
    localObj->position = centerOfMass(obj, localObj);
    localObj->mass += obj->mass;
    // insert child in its respective octant
    Octant oct = getOctant(obj);
    int index = static_cast<int>(oct);
    if (!(children[index])) {
        // a node doesn't exist here - init it
        Vec3 newCenter;
        switch (oct) {
            case Octant::FIRST: {
                newCenter = box.center + Vec3{box.width / 4.0, box.width / 4.0,
                                              box.width / 4.0};
                break;
            }
            case Octant::SECOND: {
                newCenter = box.center + Vec3{box.width / 4.0, box.width / 4.0,
                                              -1 * box.width / 4.0};
                break;
            }
            case Octant::THIRD: {
                newCenter =
                    box.center + Vec3{box.width / 4.0, -1 * box.width / 4.0,
                                      box.width / 4.0};
                break;
            }
            case Octant::FOURTH: {
                newCenter =
                    box.center + Vec3{box.width / 4.0, -1 * box.width / 4.0,
                                      -1 * box.width / 4.0};
                break;
            }
            case Octant::FIFTH: {
                newCenter = box.center + Vec3{-1 * box.width / 4.0,
                                              box.width / 4.0, box.width / 4.0};
                break;
            }
            case Octant::SIXTH: {
                newCenter =
                    box.center + Vec3{-1 * box.width / 4.0, box.width / 4.0,
                                      -1 * box.width / 4.0};
                break;
            }
            case Octant::SEVENTH: {
                newCenter =
                    box.center + Vec3{-1 * box.width / 4.0,
                                      -1 * box.width / 4.0, box.width / 4.0};
                break;
            }
            default: {
                newCenter = box.center + Vec3{-1 * box.width / 4.0,
                                              -1 * box.width / 4.0,
                                              -1 * box.width / 4.0};
                break;
            }
        }
        children[index] = new OctreeNode(box.width / 2.0, newCenter);
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
            localObj = new Object;
            // insert the old object in new octant
            insertOctant(temp);
            type = OctreeNodeType::INTERNAL;
        }
        insertOctant(obj);
    }
}

Octant OctreeNode::getOctant(Object* obj) {
    if (obj->position.x < box.center.x && obj->position.y < box.center.y &&
        obj->position.z < box.center.z) {
        // is in octant 8
        return Octant::EIGHT;
    } else if (obj->position.x < box.center.x &&
               obj->position.y < box.center.y &&
               obj->position.z >= box.center.z) {
        // is in octant 7
        return Octant::SEVENTH;
    } else if (obj->position.x < box.center.x &&
               obj->position.y >= box.center.y &&
               obj->position.z < box.center.z) {
        // is in octant 6
        return Octant::SIXTH;
    } else if (obj->position.x < box.center.x &&
               obj->position.y >= box.center.y &&
               obj->position.z >= box.center.z) {
        // is in octant 5
        return Octant::FIFTH;
    } else if (obj->position.x >= box.center.x &&
               obj->position.y < box.center.y &&
               obj->position.z < box.center.z) {
        // is in octant 4
        return Octant::FOURTH;
    } else if (obj->position.x >= box.center.x &&
               obj->position.y < box.center.y &&
               obj->position.z >= box.center.z) {
        // is in octant 3
        return Octant::THIRD;
    } else if (obj->position.x >= box.center.x &&
               obj->position.y >= box.center.y &&
               obj->position.z < box.center.z) {
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

bool OctreeNode::empty() const { return !localObj; }
const BoundingBox& OctreeNode::getBounds() const { return box; }
OctreeNodeType OctreeNode::getType() const { return type; }
const Object& OctreeNode::getObject() const {
    if (!empty())
        return *localObj;
    else
        throw runtime_error("Attempted to access empty octree node.");
}