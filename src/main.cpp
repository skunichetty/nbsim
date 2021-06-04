#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <unordered_set>

#include "engine.h"
#include "getopt.h"
#include "io_handler.h"
#include "object.h"
#include "octree.h"
#include "vec3.h"

using namespace std;

/**
 * Generates a JSON string of randomly generated objects
 */
void generateRandomObjects(stringstream& stringOutput, size_t count,
                           double width) {
    // --seed the RNG--
    random_device device;
    mt19937 twister(device());
    // --define different distributions for different values--

    // generate positions within limits of sim
    normal_distribution<double> positionGen(-1 * width, width);
    // generate velocities between 0 and 50000 m/s (fastest comet speed
    // recorded, for reference)
    normal_distribution<double> velocityGen(0, 50000);
    // generate masses between 100kg and 1e28kg (one order above Jupiter's mass)
    normal_distribution<double> massGen(100, 1e28);
    stringOutput << "{\"bodies\": [";

    // --load tree with random objects--
    for (size_t i = 0; i < count; i++) {
        stringOutput << "{\"mass\":" << massGen(twister);
        stringOutput << ",\"position\":{"
                     << "\"x\":" << positionGen(twister)
                     << ",\"y\":" << positionGen(twister)
                     << ",\"z\":" << positionGen(twister) << "}";
        stringOutput << ",\"velocity\":{"
                     << "\"x\":" << velocityGen(twister)
                     << ",\"y\":" << velocityGen(twister)
                     << ",\"z\":" << velocityGen(twister) << "}";
        stringOutput << ",\"acceleration\":{\"x\":0,\"y\":0,\"z\":0}";
        stringOutput << "}";
        if (i < count - 1) stringOutput << ",";
    }
    stringOutput << "]}";
}

int main(int argc, char** argv) {
    // ios_base::sync_with_stdio(false);
    // ----- getopt vars + options -----
    bool iflag = false;            // input from file
    bool rflag = false;            // random input
    bool inputModeChosen = false;  // is any input chosen
    bool outputChosen = false;     // a different output location specified
    bool simWidthChosen = false;   // is the simulation width manually specified
    bool verboseMode = false;      // should verbose output be printed
    int choice;
    int opt_index;
    // ----- simulation information -----
    ifstream fin;
    string finName;
    string foutName;
    ofstream fout;
    double simWidth = 1e10;
    int n = 0;
    // specify options
    option long_options[] = {{"output", required_argument, nullptr, 'o'},
                             {"input", required_argument, nullptr, 'i'},
                             {"random", required_argument, nullptr, 'r'},
                             {"width", required_argument, nullptr, 'w'},
                             {"help", no_argument, nullptr, 'h'},
                             {"verbose", no_argument, nullptr, 'v'}};
    // process options
    while ((choice = getopt_long(argc, argv, "o:i:r:w:h", long_options,
                                 &opt_index)) != -1) {
        switch (choice) {
            case 'o':
                foutName = string(optarg);
                outputChosen = true;
                break;
            case 'i':
                if (!inputModeChosen) {
                    iflag = true;
                    finName = string(optarg);
                    inputModeChosen = true;
                } else {
                    cerr << "Error: Cannot set two different input modes"
                         << endl;
                    return 1;
                }
                break;
            case 'h':
                cout << "usage: nbsim [options] <timestep> theta iterations \n";
                cout << "   arguments:\n";
                cout << setw(25) << "timestep"
                     << "\tthe amount of time between each movement of "
                        "objects. Smaller values give more accurate "
                        "results, but simulation will take longer as a "
                        "result.\n";
                cout << setw(25) << "theta"
                     << "\tparameter dictating granularity of simulation. "
                        "Limited to be in interval [0,1]. Larger value leads "
                        "to better accuracy, but significantly lower "
                        "performance. Value of 1 is equivalent to brute-force "
                        "algorithm. 0.5 is common value.\n";
                cout << setw(25) << "iterations"
                     << "\tThe number of iterations for the simulation to "
                        "run.\n";
                cout << "   options:\n";
                cout << setw(25) << "-o,--output filename"
                     << "\tSpecifies filename, the "
                        "output file for simulation results. If not specified, "
                        "prints output to console.\n ";
                cout << setw(25) << "-i,--input filename"
                     << "\tSpecifies filename, the "
                        "input file to read objects from\n";
                cout << setw(25) << "-r,--random n"
                     << "\tRandomly generates n objects to "
                        "simulate. Default simulation width is set to 1e10."
                        "but can be expanded by specifying -w option. \n";
                cout << setw(25) << "-w,--width simwidth"
                     << "\tManually specifies the width of the simulation "
                        "space in meters. If not specified, simulation space "
                        "width will "
                        "be manually determined from input.\n ";
                cout << setw(25) << "-v,--verbose"
                     << "\tPrints verbose output messages on simulation "
                        "progress\n";
                cout << setw(25) << "-h,--help"
                     << "\tPrints this help message\n";
                break;
            case 'r':
                if (!inputModeChosen) {
                    inputModeChosen = true;
                    n = atoi(optarg);
                    if (n < 0) {
                        cerr << "Error: Cannot have a negative number of "
                                "objects in simulation."
                             << endl;
                        return 1;
                    }
                    if (n > 100000) {
                        cerr << "error: Cannot have more than 100000 objects "
                                "in simulation"
                             << endl;
                        return 1;
                    }
                    rflag = true;
                } else {
                    cerr << "Error: Cannot set two different input modes"
                         << endl;
                    return 1;
                }
                break;
            case 'w':
                simWidth = (double)atof(optarg);
                simWidthChosen = true;
                if (simWidth <= 0) {
                    cerr << "Error: Cannot have a negative or zero simulation"
                            "width."
                         << endl;
                    return 1;
                }
                break;
            case 'v':
                verboseMode = true;
        }
    }
    // asserts that an input mode is chosen
    if (!inputModeChosen) {
        cerr << "Error: no input mode chosen" << endl;
        return 1;
    }

    // handles remaining arguments
    double timeStep = 1e2;
    double theta = 0.5;
    size_t iterations = 0;
    size_t index = optind;
    if (argv[index] != nullptr) {
        timeStep = atof(argv[index++]);
        if (timeStep <= 0) {
            cerr << "Error: must have time step greater than zero" << endl;
            return 1;
        }
    } else {
        cerr << "Error: Time step unspecified." << endl;
        return 1;
    }

    if (argv[index] != nullptr) {
        theta = atof(argv[index++]);
        if (theta > 1 || theta < 0) {
            cerr << "Error: theta must be in [0,1]" << endl;
            return 1;
        }
    } else {
        cerr << "Error: Theta value unspecified." << endl;
        return 1;
    }
    if (argv[index] != nullptr) {
        iterations = size_t(atoi(argv[index++]));
        if (iterations < 0) {
            cerr << "Error: no. of iterations must be positive." << endl;
            return 1;
        }
    } else {
        cerr << "Error: No. of iterations unspecified." << endl;
        return 1;
    }

    stringstream inputString;  // holds random output if used
    if (rflag) generateRandomObjects(inputString, n, simWidth);

    // all input errors resolved, open files if needed
    if (iflag) {
        fin = ifstream(finName);
        if (!fin.is_open()) {
            cerr << "Error: Could not open input file." << endl;
            return 1;
        }
    }
    if (outputChosen) {
        fout = ofstream(foutName);
        if (!fout.is_open()) {
            cerr << "Error: Could not open output file." << endl;
            return 1;
        }
    }

    // need to cast fin/fout to regular stream b/c it is a derived type
    istream& input = (iflag) ? static_cast<istream&>(fin) : inputString;
    ostream& output = (outputChosen) ? static_cast<ostream&>(fout) : cout;
    // create IOHandler
    IOHandler io(input, output);
    vector<Body> bodies;
    // load objects into vector
    size_t body_count = 0;
    try {
        while (io) {
            Body temp;
            io >> temp;
            if (simWidthChosen && (std::abs(temp.position.x) > simWidth / 2 ||
                                   std::abs(temp.position.y) > simWidth / 2 ||
                                   std::abs(temp.position.z) > simWidth / 2)) {
                string err_msg =
                    "Error: Position of object at index " +
                    to_string(body_count) +
                    " in input file has position outside of specified "
                    "simulation "
                    "bound. Either adjust simulation bound with -w "
                    "option, or specify no simulation width to automatically "
                    "determine width from given objects.";
                throw std::runtime_error(err_msg);
            }
            bodies.push_back(temp);
            index++;
        }
    } catch (exception& ee) {
        cerr << ee.what() << endl;
        return 1;
    }
    io << "{\"history\":[";
    Engine* engine;
    // if user selects a specific simulation width, make sure that width is
    // chosen
    if (simWidthChosen) {
        engine = new Engine(theta, timeStep, simWidth);
        for (auto& body : bodies) {
            engine->addBody(body);
        }
    } else {
        engine = new Engine(theta, timeStep, bodies);
    }

    for (size_t i = 0; i < iterations; i++) {
        io << engine->step();
        if (i < iterations - 1) io << ",";
        if (verboseMode) {
            cout << "Step: " << i + 1 << "/" << iterations << endl;
        }
    }
    io << "]}";

    // cleanup procedures
    fin.close();
    fout.close();
    delete engine;
    return 0;
}