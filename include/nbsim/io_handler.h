#pragma once
#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include <fstream>
#include <string>

#include "octree.h"

/**
 * Handles input/output communication. Specifically, reads input objects from
 * input stream and writes simulation state to output stream. Acts as a wrapper
 * around both an input stream and an output stream;
 */
class IOHandler {
   private:
    std::istream& infile;     // stream to read from
    std::ostream& outfile;    // stream to write to
    unsigned char inputMode;  // whether input stream is file stream or string
                              // stream. 0 = file, 1 = string

   public:
    // initializes with arbitrary streams
    IOHandler(std::istream& infileStream, std::ostream& outfileStream,
              unsigned char inputMode = 0);
    // Closes IOHandler
    ~IOHandler();
    // inserts record into output stream
    IOHandler& operator<<(const std::string output);
    // reads object from input stream
    IOHandler& operator>>(Body& body);
    // bool operator to check for eof in input
    operator bool() const;
};

#endif