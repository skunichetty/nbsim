#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "octree.h"

/**
 * Performs simulation and returns results
 */
class Engine {
   private:
    // The current time of the simulation. Starts at zero.
    float currentTime;
    // Theta parameter - dictates boundary between choosing to approximate and
    // doing full computation of force
    float theta;
    // dt parameter - dictates timestep between each evaluation. Lower dt values
    // increase numerical accuracy but also increase runtime.
    float dt;
    // Spatial tree structure.
    Octree tree;
    // Gets approximate Euclidean distance between two points in space (omits
    // the square root for speed)
    float approx_distance(const Vec3& pos1, const Vec3& pos2) const;
    // Returns the gravitational force exerted between two objects
    Vec3 forceGravity(const Object& o1, const Object& o2) const;
    // Updates the forces between all different objects in the simulation
    void updateForces(float theta);
    // Updates the motion between all different objects in the simulation
    void updateMotion(float dt);
    // Returns JSON string of current system state;
    std::string printStateJson();

   public:
    // Constructor with only default parameters
    Engine(float theta, float dt);
    // Constructor with predefined simulation width
    Engine(float theta, float dt, float simulationWidth);
    // Constructor with input of simulation bodies
    Engine(float theta, float dt, std::vector<Body>& bodies);
    // Add bodies to the simulation
    void addBody(Body& body);
    // Simulates one time step of the system
    std::string step();
};

#endif