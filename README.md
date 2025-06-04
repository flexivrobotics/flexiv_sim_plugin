# Flexiv Sim Plugin

![CMake Badge](https://github.com/flexivrobotics/flexiv_sim_plugin/actions/workflows/cmake.yml/badge.svg)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://www.apache.org/licenses/LICENSE-2.0.html)

A middleware plugin to connect Flexiv Elements Studio to any external simulator so that the Flexiv robots in this simulator are controlled by the same force-torque controller used by the real robots.

## Environment Compatibility

| **OS**                | **Platform** | **C++ compiler kit** | **Python interpreter** |
| --------------------- | ------------ | -------------------- | ---------------------- |
| Linux (Ubuntu 20.04+) | x86_64       | GCC v9.4+            | 3.8, 3.10, 3.12        |

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

## API Documentation

The API documentation can be generated using Doxygen. For example, on Linux:

    sudo apt install doxygen-latex graphviz
    cd flexiv_sim_plugin
    doxygen doc/Doxyfile.in

Open any html file under ``flexiv_sim_plugin/doc/html/`` with your browser to view the doc.
