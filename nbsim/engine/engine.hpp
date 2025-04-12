#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "nbsim/core/octree/octree.hpp"

/**
 * Performs simulation and returns results
 */
class Engine {
private:
  // The current time of the simulation. Starts at zero.
  double currentTime;
  // Theta parameter - dictates boundary between choosing to approximate and
  // doing full computation of force
  double theta;
  // dt parameter - dictates timestep between each evaluation. Lower dt values
  // increase numerical accuracy but also increase runtime.
  double dt;
  // Spatial tree structure.
  Octree tree;
  // Gets approximate Euclidean distance between two points in space (omits
  // the square root for speed)
  double approx_distance(const Vec3 &pos1, const Vec3 &pos2) const;
  // Returns the gravitational force exerted between two objects
  Vec3 accelerationGravity(const Object &o1, const Object &o2) const;
  // Updates the forces between all different objects in the simulation
  void updateForces(double theta);
  // Updates the motion between all different objects in the simulation
  void updateMotion(double dt);
  // Returns JSON string of current system state;
  std::string printStateJson();
  // Computes the force exerted on the object obj by all other bodies in the
  // tree
  void computeForce(OctreeNode *root, Body &obj);

public:
  // Constructor with only default parameters
  Engine(double theta, double dt);
  // Constructor with predefined simulation width
  Engine(double theta, double dt, double simulationWidth);
  // Constructor with input of simulation bodies
  Engine(double theta, double dt, std::vector<Body> &bodies);
  // Add bodies to the simulation
  void addBody(Body &body);
  // Simulates one time step of the system
  std::string step();
};

#endif