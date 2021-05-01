#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "octree.h"

class Engine {
   private:
    // Theta parameter - dictates boundary between choosing to approximate and
    // doing full computation of force
    double theta;
    // dt parameter - dictates timestep between each evaluation. Lower dt values
    // increase numerical accuracy but also increase runtime.
    double dt;
    // The width of the simulation space, only record 1 dimension because the
    // simulation space is a cube
    double simulationWidth;
    // Underlying tree structure. Dynamically allocated to enable
    Octree tree;

   public:
    Engine(double theta, double dt, double simulationWidth);
    void simulate(double timeLength);
    void addBody(Object& object);
};

#endif