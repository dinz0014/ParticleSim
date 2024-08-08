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
