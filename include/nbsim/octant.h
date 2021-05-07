#pragma once
#ifndef OCTANTS_H
#define OCTANTS_H
/**
 * Represents the eight octants in space. Provides user (and programmer)
 * friendly names to deal with relative locations in simulation space
 */
enum class Octant : int {
    FIRST = 0,
    SECOND = 1,
    THIRD = 2,
    FOURTH = 3,
    FIFTH = 4,
    SIXTH = 5,
    SEVENTH = 6,
    EIGHT = 7
};
#endif