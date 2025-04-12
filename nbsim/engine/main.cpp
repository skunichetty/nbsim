#include <bitset>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#include "getopt.h"
#include "nbsim/core/octree/object.hpp"
#include "nbsim/core/octree/octree.hpp"
#include "nbsim/core/vec3/vec3.hpp"
#include "nbsim/engine/engine.hpp"
#include "nbsim/engine/io_handler.hpp"

using namespace std;

/**
 * Generates a JSON string of randomly generated objects
 */
void generateRandomObjects(stringstream& stringOutput, size_t count) {
    // --seed the RNG--
    random_device device;
    mt19937 twister(device());
    // generate positions within limits of sim
    uniform_real_distribution<double> positionGen(-1 * 1e20, 1e20);
    // generate velocities between 0 and 50000 m/s (fastest comet speed
    // recorded, for reference)
    uniform_real_distribution<double> velocityGen(0, 50000);
    // generate masses between 100kg and 1e28kg (one order above Jupiter's mass)
    uniform_real_distribution<double> massGen(100, 1e28);
    stringOutput << "{\"bodies\": [";
    // load string with the values computed
    for (size_t i = 0; i < count; i++) {
        stringOutput << "{\"mass\":" << massGen(twister);
        stringOutput << ",\"position\":{"
                     << "\"x\":" << positionGen(twister) << ",\"y\":" << positionGen(twister)
                     << ",\"z\":" << positionGen(twister) << "}";
        stringOutput << ",\"velocity\":{"
                     << "\"x\":" << velocityGen(twister) << ",\"y\":" << velocityGen(twister)
                     << ",\"z\":" << velocityGen(twister) << "}";
        stringOutput << ",\"acceleration\":{\"x\":0,\"y\":0,\"z\":0}";
        stringOutput << "}";
        if (i < count - 1)
            stringOutput << ",";
    }
    stringOutput << "]}";
}

struct NbsimOptions {
    /**
     * bit options for chosen parameters. The bits read as:
     * 0 - input file provided
     * 1 - random input chosen
     * 2 - any input chosen
     * 3 - any output chosen
     * 4 - is verbose mode enabled
     */
    bitset<5> options;
    int nRand = 0;         // Number of planets to randomly generate
    size_t iterations = 0; // no. of iterations
    double timeStep = 1e2; // timestep to follow
    double theta = 0.5;    // theta param - level of approximation
    string finName;        // input filename
    string foutName;       // output filename
};

class Vec3HashFunction {
  private:
    hash<double> hasher;
    const size_t C = 0x9e3779b9; // large prime
  public:
    size_t operator()(const Vec3& vec) const {
        size_t seed = 0;
        seed ^= hasher(vec.x) + C + (seed << 6) + (seed >> 2);
        seed ^= hasher(vec.y) + C + (seed << 6) + (seed >> 2);
        seed ^= hasher(vec.z) + C + (seed << 6) + (seed >> 2);
        return seed;
    }
};

void printHelp() {
    cout << "usage: nbsim [options] <timestep> theta iterations \n";
    cout << "   arguments:\n";
    cout << setw(25) << "timestep"
         << "\tthe amount of time between each movement of objects.\n";
    cout << setw(25) << "theta"
         << "\tparameter dictating granularity of simulation\n";
    cout << setw(25) << "iterations"
         << "\tThe number of iterations for the simulation to run.\n";
    cout << "   options:\n";
    cout << setw(25) << "-o,--output filename"
         << "\tSpecifies filename, the output file for simulation results.\n ";
    cout << setw(25) << "-i,--input filename"
         << "\tSpecifies filename, the input file to read objects from\n";
    cout << setw(25) << "-r,--random n"
         << "\tRandomly generates n objects to simulate.\n";
    cout << setw(25) << "-v,--verbose"
         << "\tPrints verbose output messages on simulation progress\n";
    cout << setw(25) << "-h,--help"
         << "\tPrints this help message\n";
}

/**
 * Gets user specified options. If unable to process user options, will throw
 * std::runtime_error.
 */
NbsimOptions getOptions(int argc, char** argv) {
    NbsimOptions options;
    // ---- GETOPT OPTION HANDLING ----
    int choice;
    int opt_index;
    option long_options[] = {
        {"output",  required_argument, nullptr, 'o'},
        {"input",   required_argument, nullptr, 'i'},
        {"random",  required_argument, nullptr, 'r'},
        {"help",    no_argument,       nullptr, 'h'},
        {"verbose", no_argument,       nullptr, 'v'}
    };
    while ((choice = getopt_long(argc, argv, "o:i:r:hv", long_options, &opt_index)) != -1) {
        switch (choice) {
        case 'o':
            options.foutName = string(optarg);
            options.options[3] = true;
            break;
        case 'i':
            if (!options.options[2]) {
                options.options[0] = true;
                options.finName = string(optarg);
                options.options[2] = true;
            } else {
                throw std::runtime_error("Cannot set two different input modes");
            }
            break;
        case 'h':
            printHelp();
            break;
        case 'r':
            if (!options.options[2]) {
                options.options[2] = true;
                size_t n = atoi(optarg);
                if (n < 0) {
                    throw std::runtime_error("Cannot have a negative number of objects in simulation.");
                }
                if (n > 100000) {
                    throw std::runtime_error("Cannot have more than 100000 objects in simulation");
                }
                options.nRand = n;
                options.options[1] = true;
            } else {
                throw std::runtime_error("Cannot set two different input modes");
            }
            break;
        case 'v':
            options.options[4] = true;
        }
    }
    // asserts that an input mode is chosen
    if (!options.options[2]) {
        throw std::runtime_error("No input mode chosen");
    }

    // ---- REMAINDER PARAMETER HANDLING ----
    size_t index = optind;
    if (argv[index] != nullptr) {
        options.timeStep = atof(argv[index++]);
        if (options.timeStep <= 0) {
            throw std::runtime_error("Must have time step greater than zero");
        }
    } else {
        throw std::runtime_error("Time step unspecified.");
    }

    if (argv[index] != nullptr) {
        options.theta = atof(argv[index++]);
        if (options.theta < 0) {
            throw std::runtime_error("Theta must be greater than zero.");
        }
    } else {
        throw std::runtime_error("Theta value unspecified.");
    }
    if (argv[index] != nullptr) {
        options.iterations = size_t(atoi(argv[index++]));
        if (options.iterations < 0) {
            throw std::runtime_error("No. of iterations must be positive.");
        }
    } else {
        throw std::runtime_error("No. of iterations unspecified.");
    }
    return options;
}

Engine* setupEngine(const NbsimOptions& options, IOHandler& io) {
    vector<Body> bodies;
    unordered_set<Vec3, Vec3HashFunction> positions;
    // load objects into vector
    size_t body_count = 0;
    double max_coord = 0;
    while (io) {
        Body temp;
        io >> temp;
        auto duplicateIter = positions.find(temp.position);
        if (duplicateIter != positions.end()) {
            // Hash check triggered - do finer comparision to make sure they
            // actually conflict
            if (temp.position == *duplicateIter && options.options[0]) {
                ostringstream str;
                str << "Body at index " << body_count << " in " << options.finName
                    << " has the same position as another body, which is "
                       "not allowed.";
                throw std::runtime_error(str.str());
            }
        }
        double max_obj_coord = max(abs(temp.position.x), max(abs(temp.position.y), abs(temp.position.z)));
        max_coord = max(max_coord, max_obj_coord);
        bodies.push_back(temp);
        positions.insert(temp.position);
        body_count++;
    }
    Engine* engine = new Engine(options.theta, options.timeStep, max_coord);
    for (Body& body : bodies) {
        engine->addBody(body);
    }
    return engine;
}

int main(int argc, char** argv) {
#ifdef NDEBUG
    // sync w/ stdio on debug in order to appease valgrind
    ios_base::sync_with_stdio(false);
#endif
    NbsimOptions options;
    IOHandler* io = nullptr;
    Engine* engine = nullptr;
    ifstream fin;
    ofstream fout;
    stringstream inputString; // holds random output if used
    try {
        options = getOptions(argc, argv);
        if (options.options[1])
            generateRandomObjects(inputString, options.nRand);

        // all input errors resolved, open files if needed
        if (options.options[0]) {
            fin.open(options.finName);
            if (!fin.is_open()) {
                throw std::runtime_error("Could not open input file.");
            }
        }
        if (options.options[2]) {
            fout.open(options.foutName);
            if (!fout.is_open()) {
                throw std::runtime_error("Could not open output file.");
            }
        }

        // need to cast fin/fout to regular stream b/c it is a derived type
        istream& input = (options.options[0]) ? static_cast<istream&>(fin) : inputString;
        ostream& output = (options.options[2]) ? static_cast<ostream&>(fout) : cout;
        io = new IOHandler(input, output);
        engine = setupEngine(options, *io);
    } catch (std::exception& e) {
        cerr << "ERROR:" << e.what() << endl;
        return 1;
    }
    size_t iterations = options.iterations;
    *io << "{\"history\":[";
    for (size_t i = 0; i < iterations; i++) {
        *io << engine->step();
        if (i < iterations - 1)
            *io << ",";
        if (options.options[4]) {
            cout << "Step: " << i + 1 << "/" << iterations << endl;
        }
    }
    *io << "]}";

    // cleanup procedures
    delete engine;
    delete io;
    return 0;
}