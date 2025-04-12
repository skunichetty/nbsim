# nbsim: A N-Body Simulation
N-Body gravitational simulation software in C++, built with performance and scaling in mind for large scale simulations.

> [!NOTE]
> `nbsim` at its core is a side project worked on by enthusiasts. If you need a production ready solution, it is recommended to search for another solution until `nbsim` is more mature.

## Setup

`nbsim` uses [Bazel](https://bazel.build/) to manage the build process. To compile the main binary, run:

```sh
bazel build //nbsim/engine/main
```

Bazel will install relevant dependencies (like GTest) and compile the main binary. The resulting binary can be found (by default) at `bazel-bin/nbsim/engine/main`.

## Quick Start

### Fixed Input

The intended workflow for `nbsim` is for users to provide a set of initial body definitions, from which the software will start simulating gravitational motion.

Consider the following definitions defining a simplified model of lunar orbit[^1]:
```javascript 
{
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

Run the following command to provide `nbsim` with these definitions and start simulating gravitational motion.

```sh
nbsim 10 0.5 100 -i input.json -o temp.json
```

To breakdown some of the parameters:
- `10`: the number of seconds encompassing each simulation step
- `0.5`: is $\theta \in [0, 1]$, the granularity of the simulation. Higher values of $\theta$ lead to more precision in simulation calculations at the cost of runtime.
- `100`: the number of steps to run the simulation for

The simulation results are saved to `temp.json`.

### Random Input

For testing purposes, `nbsim` supports running the main binary with randomly generated bodies. This option is recommended when you are interested in tinkering with the program.

The following command runs `nbsim` for 100 iterations (each 10 seconds long) at default granularity with 20 randomly generated objects. The simulation results are saved to `temp.json`.

```sh
nbsim 10 0.5 100 -r 20 -o temp.json
```

## Full Options List
- `-o,--output <filename>` Specifies filename, the output file for simulation results. If not specified,prints output to console.
- `-i,--input <filename>` Specifies filename, the input file to read objects from
- `-r,--random <n>` Randomly generates n objects to simulate. Default simulation width is set to 1e10 meters, but can be expanded by specifying the -w option
- `-v,--verbose` Prints verbose output messages on simulation progress.
- `-h,--help` Prints a help message listing options and arguments.

[^1]: The moon's radius of orbit is on average 1,737 kilometers and its velocity is on average 1,022 meters per second. While not wholly circular, as assumed for there to be a fixed orbit, choosing an approximate starting radius should allow for approximate behavior to be simulated.