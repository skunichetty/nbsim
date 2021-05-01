#include "engine.h"

#include <iostream>

using namespace std;

Engine::Engine(double theta, double dt, double simulationWidth)
    : theta{theta},
      dt{dt},
      simulationWidth{simulationWidth},
      tree{Octree(525600)} {}

void Engine::addBody(Object& object) { tree.insert(object); }

void Engine::simulate(double timeLength) {
    double currentTime = 0;
    cout << "======TIME:" << currentTime << "======\n";
    tree.printSummary();
    while (currentTime < timeLength) {
        tree.update(theta, dt);
        currentTime += dt;
        cout << "======TIME:" << currentTime << "======\n";
        tree.printSummary();
        tree.rebuildTree();
    }
}