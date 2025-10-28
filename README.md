# Flexiv Sim Plugin

![CMake Badge](https://github.com/flexivrobotics/flexiv_sim_plugin/actions/workflows/cmake.yml/badge.svg)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://www.apache.org/licenses/LICENSE-2.0.html)

A middleware plugin to connect Flexiv Elements Studio to any external simulator so that the Flexiv robots in this simulator are controlled by the same force-torque controller used by the real robots.

## Environment Compatibility

| **OS**                | **Platform** | **C++ compiler kit** | **Python interpreter** |
| --------------------- | ------------ | -------------------- | ---------------------- |
| Linux (Ubuntu 20.04+) | x86_64       | GCC v9.4+            | 3.8, 3.10, 3.12        |

## Tested External Simulators

The following external simulators are tested and known to work with Flexiv Sim Plugin:

- NVIDIA [Isaac Sim](https://developer.nvidia.com/isaac/sim) (a template workspace is available [here](https://github.com/flexivrobotics/isaac_sim_ws))
- [MuJoCo](https://mujoco.org/)

In theory, any simulator that meets the following criteria should work:

1. Has a C++ or Python interface.
2. Provides joint positions and velocities of the simulated robot.
3. The joints of the simulated robot are actuated by torque.

## Demos

Below are some demos where Flexiv Elements Studio is connected to NVIDIA Isaac Sim to simulate applications where force control is required. The full demo videos can be found [here](https://github.com/flexivrobotics/isaac_sim_ws?tab=readme-ov-file#demos).

### Tower of Hanoi

![Tower of Hanoi](doc/demo/tower_of_hanoi.gif)

### Peg-in-hole

![Peg-in-hole](doc/demo/peg_in_hole.gif)

### Dual-robot polish

![Dual-robot polish](doc/demo/dual_arm_polish.gif)

## Flexiv Elements Studio Setup

### Install Elements Studio

1. Prepare a Ubuntu 22.04 computer, all operations below are done on this computer.
2. [Contact Flexiv](https://www.flexiv.com/contact) to obtain the installation package of Elements Studio.
3. Extract the package to a non-root directory.
4. Install Elements Studio:

       bash setup_FlexivElements.sh

5. Switch physics engine from the default built-in to external:

       bash switch_physics_engine.sh

   Select *External* or *Isaac Sim* when prompted.

### Create a simulated robot in Elements Studio

1. Start Flexiv Elements Studio from the application menu.
2. In the Robot Connection window, select *Simulator*, and click *CREATE*.
3. Choose "Create according to the selected robot type" and select one from the list, then click *CONFIRM*. A new simulated robot will be added to the simulator list.
4. Toggle on the *Connect* button for the newly added one, then wait for loading.
5. When loading is finished, you'll see a robot at its upright pose, with an "Exception" error at the bottom right corner. This is expected because the external simulator is not started yet. But if you see a normally operating robot, that again means you are running the wrong version of Elements Studio that only supports the built-in physics engine.
6. At the bottom of the window, click on the small robot icon with a "SIM" tag on it, then a small window will pop up, note down the displayed robot serial number.
7. In the same small pop-up window, click *CHANGE CONNECTION*, then toggle off the *Connect* button to close the simulated robot. We will restart it later. Note that you do NOT need to close the whole Elements Studio program.

## Quick Start - C++

### Prepare build tools

#### Linux

1. Install compiler kit using package manager:

       sudo apt install build-essential

2. Install CMake using package manager:

       sudo apt install cmake

### Install the C++ library

The following steps are identical on all supported platforms.

1. Choose a directory for installing the C++ library of Sim Plugin and its dependencies. This directory can be under system path or not, depending on whether you want Sim Plugin to be globally discoverable by CMake. For example, a new folder named ``sim_plugin_install`` under the home directory.
2. In a new Terminal, run the provided script to compile and install all dependencies to the installation directory chosen in step 1:

       cd flexiv_sim_plugin/thirdparty
       bash build_and_install_dependencies.sh ~/sim_plugin_install

3. In a new Terminal, configure the ``flexiv_sim_plugin`` CMake project:

       cd flexiv_sim_plugin
       mkdir build && cd build
       cmake .. -DCMAKE_INSTALL_PREFIX=~/sim_plugin_install

   NOTE: ``-D`` followed by ``CMAKE_INSTALL_PREFIX`` sets the absolute path of the installation directory, which should be the one chosen in step 1.

4. Install ``flexiv_sim_plugin`` C++ library to ``CMAKE_INSTALL_PREFIX`` path, which may or may not be globally discoverable by CMake:

       cd flexiv_sim_plugin/build
       cmake --build . --target install --config Release

### Use the installed C++ library

After the library is installed as ``flexiv_sim_plugin`` CMake target, it can be linked from any other CMake projects. Using the provided `flexiv_sim_plugin-examples` project for instance:

    cd flexiv_sim_plugin/example
    mkdir build && cd build
    cmake .. -DCMAKE_PREFIX_PATH=~/sim_plugin_install
    cmake --build . --config Release -j 4

NOTE: ``-D`` followed by ``CMAKE_PREFIX_PATH`` tells the user project's CMake where to find the installed C++ library. This argument can be skipped if the Sim Plugin library and its dependencies are installed to a globally discoverable location.

### Run the example C++ program

An example program that mocks an external simulator is provided and can be used to test the plugin with the following steps:

1. Start the mock program:

       cd flexiv_sim_plugin/example/build
       ./mock_external_simulator [robot_serial_number]

   NOTE: the robot serial number provided to the program is the same one you noted down when creating the simulated robot in Flexiv Elements Studio.

2. Go back to Elements Studio, then restart the exited simulator by toggling ON the *Connect* button.
3. Wait for the connection to establish. If the connection is successful, you should see the visualized robot in Elements Studio moving every joint back and forth. NOTE: a software error should occur in Elements Studio which is expected because the mock external simulator did not close the loop by applying the calculated joint torques command to the simulated robot in it. This won't happen to real external simulators.

## Quick Start - Python

### Install the Python package

On all supported platforms, the Python package of Sim Plugin for a specific Python version can be installed using the `pip` module:

    python3.x -m pip install flexivsimplugin

NOTE: replace `3.x` with a specific Python version.

### Use the installed Python package

After the ``flexivsimplugin`` Python package is installed, it can be imported from any Python script. Test with the following commands in a new Terminal, which should start Flexiv Sim Plugin:

    python3.x
    import flexivsimplugin
    node = flexivsimplugin.UserNode("Rizon4-123456")
    print("Connected = ", node.connected())

The program should print some info messages and "Connected = False" at the end.

### Run the example Python script

An example script that mocks an external simulator is provided and can be used to test the plugin with the following steps:

1. Start the mock program:

       cd flexiv_sim_plugin/example_py
       python3.x ./mock_external_simulator.py [robot_serial_number]

   NOTE: the robot serial number provided to the program is the same one you noted down when creating the simulated robot in Flexiv Elements Studio.

2. The remaining steps are the same as documented in [Run the example C++ program](#run-the-example-c-program).

## API Documentation

The API documentation can be generated using Doxygen. For example, on Linux:

    sudo apt install doxygen-latex graphviz
    cd flexiv_sim_plugin
    doxygen doc/Doxyfile.in

Open any html file under ``flexiv_sim_plugin/doc/html/`` with your browser to view the doc.
