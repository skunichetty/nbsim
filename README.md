# nbsim: A N-Body Simulation
A simulation of the gravitational interactions of n bodies in space. This problem is known to show chaotic behavior (highly dependent on initial conditions), which makes its study particularly fascinating.

## Setup
To get started, clone this repo and compile the code. With g++:
`g++ -O3 -I ./include/nbsim src/* -o nbsim`
Note that the `-O3` flag is important here to maximize performance. If using other compilers, make sure the optimization setting is at max.
Then to run the program, run:
`./nbsim <timestep> <theta> <iterations> [options]`
where:
- `timestep` - the amount of time elapsed between each simulation evalution in seconds.
- `theta` - parameter dictating granularity of simulation. Limited to be in interval [0,1]. Larger value leads to better accuracy, but significantly lower performance. Value of 1 is equivalent to brute-force algorithm. 0.5 is common value.
- `iterations` - the number of iterations to run the simulation for.
Run `./nbsim -h` to learn more about options.

## Quick Start
Here are some commands to get started, for those who don't have time to learn heavily about the different options:
- To run with random input:
`./nbsim 10 0.5 100 -r 10 -o temp.json`
This will randomly generate 10 objects, simulate their gravitational iteraction for 10 iterations with 10 seconds in between each evaluation, and then outputs the simulation results to the "temp.json" file.
- To run with custom input:
`./nbsim 10 0.5 100 -i input.json -o temp.json`
This will simulate the gravitational interactions between the objects specified in "input.json" for 10 iterations with 10 seconds in between each evaluation, and then outputs the simulation results to the "temp.json" file.
A sample "input.json" is given here: (Earth and Moon*)
```javascript {
    "bodies": [
        {
            "mass": 5.972e24,
            "position": {
                "x": 0,
                "y": 0,
                "z": 0
            },
            "velocity": {
                "x": 0,
                "y": 0,
                "z": 0
            },
            "acceleration": {
                "x": 0,
                "y": 0,
                "z": 0
            }
        },
        {
            "mass": 7.347e22,
            "position": {
                "x": 0,
                "y": 1737100,
                "z": 0
            },
            "velocity": {
                "x": 1022,
                "y": 0,
                "z": 0
            },
            "acceleration": {
                "x": 0,
                "y": 0,
                "z": 0
            }
        }
    ]
}
```
\* \- The moon's radius is on average 1,737 kilometers and its velocity is on average 1,022 meters per second. The orbit isn't exactly elliptical, but this approximation should suffice.

## Full Options List
- `-o,--output <filename>` Specifies filename, the output file for simulation results. If not specified,prints output to console.
- `-i,--input <filename>` Specifies filename, the input file to read objects from
- `-r,--random <n>` Randomly generates n objects to simulate. Default simulation width is set to 1e10 meters, but can be expanded by specifying the -w option
- `-w,--width <simwidth>` Manually specifies the width of the simulation space in meters. If not specified, simulation space width will be manually determined from input.
- `-v,--verbose` Prints verbose output messages on simulation progress.
- `-h,--help` Prints a help message listing options and arguments.