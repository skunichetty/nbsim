#pragma once
#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include <fstream>
#include <string>

#include "nbsim/core/octree/octree.hpp"

/**
 * Handles input/output communication. Specifically, reads input objects from
 * input stream and writes simulation state to output stream. Acts as a wrapper
 * around both an input stream and an output stream;
 */
class IOHandler {
  private:
    std::istream& infile;  // stream to read from
    std::ostream& outfile; // stream to write to

  public:
    // initializes with arbitrary streams
    IOHandler(std::istream& infileStream, std::ostream& outfileStream);
    // inserts record into output stream
    IOHandler& operator<<(const std::string output);
    // reads object from input stream
    IOHandler& operator>>(Body& body);
    // bool operator to check for eof in input
    operator bool() const;
};

#endif