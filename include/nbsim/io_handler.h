#pragma once
#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include <fstream>

#include "octree.h"

/**
 * Handles input/output communication. Specifically, reads input objects from
 * input stream and writes simulation state to output stream
 */
class IOHandler {
   private:
    std::istream& infile;   // stream to read from
    std::ostream& outfile;  // stream to write to

   public:
    // initializes with arbitrary streams
    IOHandler(std::istream& infileStream, std::ostream& outfileStream);

    // inserts record into output stream
    IOHandler& operator<<(const Octree& octree);
    // reads object from input stream
    IOHandler& operator>>(Object& obj);
};

#endif