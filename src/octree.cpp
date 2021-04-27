#include "octree.h"

using namespace std;

Octree::~Octree() { delete root; }

Octree::Octree(double simWidth, vector<Object>& objects)
    : simWidth{simWidth}, root{nullptr}, objects{objects} {
    buildTree();
}

void Octree::buildTree() {
    delete root;
    Vec3 center = {0, 0, 0};
    root = new OctreeNode(OctreeNodeType::EXTERNAL, simWidth, center);
    for (auto& val : objects) {
        root->insert(&val);
    }
}