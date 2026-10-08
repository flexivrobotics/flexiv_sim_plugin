# Flexiv Sim Plugin

![Cpp Badge](https://github.com/flexivrobotics/flexiv_sim_plugin/actions/workflows/ci-cpp.yml/badge.svg)
![Python Badge](https://github.com/flexivrobotics/flexiv_sim_plugin/actions/workflows/ci-python.yml/badge.svg)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://www.apache.org/licenses/LICENSE-2.0.html)


## Overview

Flexiv Sim Plugin is a C++/Python library that bridges **Flexiv Elements Studio** and any external physics simulator, so that simulated Flexiv robots are driven by the same high-performance force-torque controller used on real robots — without requiring you to deal with the underlying IPC.

```
┌──────────────────────────────────────────┐
│       User Program with RDK Client       │
│          (send robot commands)           │
└────────────────────┬─────────────────────┘
                     │ RDK API
                     ▼
┌──────────────────────────────────────────┐
│         Flexiv Elements Studio           │
│   (simulated controller + RDK Server)    │
└────────────────────┬─────────────────────┘
                     │ Flexiv Sim Plugin API
                     ▼
┌────────────────────────────────────────────┐
│          External Simulator                |
│ (Flexiv Sim Plugin API + External sim API) │
└────────────────────────────────────────────┘
```

## Supported External Simulators

The following external simulators are tested and known to work with Flexiv Sim Plugin:

- NVIDIA [Isaac Sim](https://developer.nvidia.com/isaac/sim) (a template workspace is available [here](https://github.com/flexivrobotics/isaac_sim_ws))
- [MuJoCo](https://mujoco.org/)

In theory, any simulator that meets the following criteria should work:

1. Has a C++ or Python interface.
2. Provides joint positions and velocities of the simulated robot.
3. Actuates the joints of the simulated robot by torque.


## Environment Compatibility

| **OS**                | **Platform** | **C++ compiler kit** | **Python interpreter** |
| --------------------- | ------------ | -------------------- | ---------------------- |
| Linux (Ubuntu 22.04+) | x86_64       | GCC v11.4+           | 3.10, 3.12, 3.14       |

Each release line of Flexiv Sim Plugin works with one release line of Flexiv Elements Studio, because the two must use the same transport and messages:

| **flexiv_sim_plugin** | **Elements Studio** | **Transport** | **Branch** |
| --------------------- | ------------------- | ------------- | ---------- |
| 2.2.x                 | v3E.2               | Zenoh         | `v2.x`     |
| 2.1.x                 | v3E.1               | Fast-DDS      | `v2.1.x`   |
| 1.3.0                 | v3.11.2             | Fast-DDS      | `v1.x`     |

This branch, `v2.1.x`, is the 2.1 line for Elements Studio v3E.1. It finds Elements Studio by Fast-DDS discovery, which multicasts to `239.255.0.1` on UDP port 14900 (DDS domain 30), so the external simulator and Elements Studio must be on a network that passes multicast. A VPN, or Docker's default bridge network, can prevent the connection; run a container on the host network.

`SimRobotStates::wrist_force` and `wrist_torque` are accepted, so the same code builds against every 2.x release, but Elements Studio v3E.1 doesn't receive them: the plugin logs a warning once and drops them.


## Quick Start - Python

### Install the Python package

On all supported platforms, the Python package of Sim Plugin for a specific Python version can be installed using the `pip` module:

    python3 -m pip install flexivsimplugin

NOTE: replace python3 with a specific version (e.g. python3.10) if your default python3 is not one of the versions listed in the table above.

### Use the installed Python package

After the ``flexivsimplugin`` Python package is installed, it can be imported from any Python script. Test with the following commands in a new Terminal, which should start Flexiv Sim Plugin:

    python3
    import flexivsimplugin
    node = flexivsimplugin.UserNode("Rizon4-123456")
    print("Connected = ", node.connected())

The program should print some info messages and "Connected = False" at the end.

### Run the example Python script

An example script that mocks an external simulator is provided and can be used to test the plugin with the following steps:

1. Setup and run Flexiv Elements Studio simulation. See [docs/elements_studio_setup.md](docs/elements_studio_setup.md).

2. Start the mock program:

       cd flexiv_sim_plugin/example_py
       python3 ./mock_external_simulator.py [robot_serial_number]

   NOTE: the robot serial number provided to the program is the same one you noted down when creating the simulated robot in Flexiv Elements Studio.

3. Wait for the connection to establish. If the connection is successful, you should see the visualized robot in Elements Studio moving every joint back and forth. NOTE: a software error should occur in Elements Studio which is expected because the mock external simulator did not close the loop by applying the calculated joint torques command to the simulated robot in it. This won't happen to real external simulators.


## Quick Start - C++

### Prepare build tools

#### Linux

1. Install compiler kit using package manager:

       sudo apt install build-essential

2. Install CMake using package manager:

       sudo apt install cmake

### Install the C++ library

The following steps are identical on all supported platforms.

The C++ library is a self-contained shared library: its dependencies are built in, so there is nothing else to install first.

1. Choose a directory for installing the C++ library of Sim Plugin. This directory can be under system path or not, depending on whether you want Sim Plugin to be globally discoverable by CMake. For example, a new folder named ``sim_plugin_install`` under the home directory.

2. In a new Terminal, configure the ``flexiv_sim_plugin`` CMake project. This downloads the prebuilt library from the matching GitHub release and verifies its checksum:

       cd flexiv_sim_plugin
       mkdir build && cd build
       cmake .. -DCMAKE_INSTALL_PREFIX=~/sim_plugin_install

   NOTE: ``-D`` followed by ``CMAKE_INSTALL_PREFIX`` sets the absolute path of the installation directory, which should be the one chosen in step 1.

3. Install ``flexiv_sim_plugin`` C++ library to ``CMAKE_INSTALL_PREFIX`` path, which may or may not be globally discoverable by CMake:

       cd flexiv_sim_plugin/build
       cmake --build . --target install --config Release

### Use the installed C++ library

After the library is installed as ``flexiv_sim_plugin`` CMake target, it can be linked from any other CMake projects. Using the provided `flexiv_sim_plugin-examples` project for instance:

    cd flexiv_sim_plugin/example
    mkdir build && cd build
    cmake .. -DCMAKE_PREFIX_PATH=~/sim_plugin_install
    cmake --build . --config Release -j 4

NOTE: ``-D`` followed by ``CMAKE_PREFIX_PATH`` tells the user project's CMake where to find the installed C++ library. This argument can be skipped if the Sim Plugin library is installed to a globally discoverable location.

### Run the example C++ program

An example program that mocks an external simulator is provided and can be used to test the plugin with the following steps:

1. Setup and run Flexiv Elements Studio simulation. See [docs/elements_studio_setup.md](docs/elements_studio_setup.md).

2. Start the mock program. The install location of the Sim Plugin shared library is baked into the executable as an RPATH, so it is found automatically at runtime with no extra setup:

       cd flexiv_sim_plugin/example/build
       ./mock_external_simulator [robot_serial_number]

   NOTE: the robot serial number provided to the program is the same one you noted down when creating the simulated robot in Flexiv Elements Studio.

3. Wait for the connection to establish. If the connection is successful, you should see the visualized robot in Elements Studio moving every joint back and forth. NOTE: a software error should occur in Elements Studio which is expected because the mock external simulator did not close the loop by applying the calculated joint torques command to the simulated robot in it. This won't happen to real external simulators.

## API Documentation

The API documentation can be generated using Doxygen. For example, on Linux:

    sudo apt install doxygen-latex graphviz
    cd flexiv_sim_plugin
    doxygen doc/Doxyfile.in

Open any html file under ``flexiv_sim_plugin/doc/html/`` with your browser to view the doc.

