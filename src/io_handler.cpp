#include "io_handler.h"

using namespace std;

IOHandler::IOHandler(std::istream& infileStream, std::ostream& outfileStream)
    : infile{infileStream}, outfile{outfileStream} {}

IOHandler& IOHandler::operator<<(const std::string output) {
    outfile << output;
    return *this;
}

IOHandler& IOHandler::operator>>(Body& body) {
    float mass = 0;
    char junk = '\0';
    // position object at correct location
    while (infile >> junk && junk != 'm') continue;
    // move to start
    while (infile >> junk && junk != ':') continue;
    infile >> mass;
    body.mass = mass;
    // load vectors
    Vec3 vectors[3];
    for (size_t i = 0; i < 3; i++) {
        float x = 0, y = 0, z = 0;
        while (infile >> junk && junk != 'x') continue;
        infile >> junk >> junk >> x;
        while (infile >> junk && junk != 'y') continue;
        infile >> junk >> junk >> y;
        while (infile >> junk && junk != 'z') continue;
        infile >> junk >> junk >> z;
        vectors[i] = Vec3{x, y, z};
    }
    body.position = vectors[0];
    body.velocity = vectors[1];
    body.acceleration = vectors[2];
    // position to end of next string
    infile >> junk >> junk;
    return *this;
}

IOHandler::operator bool() const {
    char junk;
    if (infile >> junk) {
        if (junk == ']')
            return false;
        else
            return true;
    } else {
        return false;
    }
}