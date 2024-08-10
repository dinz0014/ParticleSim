# Particle Simulation using C++

A simple simulation that uses Verlet integration to update particle positions for circular particle models with configurable controls. Compatible with **MSVC** or **GCC/CLANG** on **Windows 10**

## Installation

Installation is pretty simple. You need to download and install **vcpkg** somewhere on your computer. This project uses vcpkg for package management.

You will also need a C++ compiler like MSVC or GCC. I have not tested this on Linux, but on Windows 10, the installation steps should *just work*

1. Clone this repository
2. Download and Install ```vcpkg```
3. In your environment variables, set ```VCPKG_ROOT=[path_to_root_of_vcpkg_install]```
4. Create a directory ```build``` in the root of this repo, and ```cd``` into it.
5. Run ```cmake ../```
6. Run ```cmake --build build --config Release```

The release binary should now be present in ```build/src/app/Release/ParticleSimulation.exe```

## How To Use

1. Left clicking should create particles at the cursor.
2. You can left-click and drag to create a lot of particles quickly.
3. Pressing P pauses the simulation, and pressing R resets the simulation
4. Various controls can be configured inside ```sim_config.json```
5. After adjusting the config file, you must restart the application (More convenient config loading coming soon)

## Configuration

The ```sim_config.json``` file has the following allowed fields. This file is copied into the simulation binary's directory by ```CMake``` during compilation. Make sure that the changes you make are to the config file in the binary's directory. More convenience features will be available in the future.

```json
{
    "app" : {
        "solver_substeps" : 16, // The number of iterations the solver should run for every frame [default = 16]
        "target_fps" : 60 // The FPS you want the app to target. [default = 60]
    },

    "particle" : {
        "radius" : 10.0, // The radius of all particles [default = 10.0]
        "colour" : "#00FFFF", // The hexcode colour of all particles [default = #00FFFF (Cyan)]

        // If the relative velocity change in a particular axis is below this threshold, the velocity along that axis will not change.
        // This applies during particle-particle collisions and verlet update step [default = 0.001]
        "rel_spd_change_threshold" : 0.001,

        // Same as above, but applies only during wall collisions [default = 0.01]
        "rel_rb_spd_change_threshold" : 0.01
    },

    "physics" : {
        "grav_accel" : 800.0, // The acceleration due to gravity (downwards) [default = 800.0]
        "wall_collision_damping_constant" : 0.85, // The factor by which the velocity is dampened during particle-wall collisions
        "max_1d_spd" : 900.0 // The maximum one directional speed for particles
    }
}
```
