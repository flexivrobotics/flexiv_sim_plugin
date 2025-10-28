#!/bin/bash
# Depends on: foonathan_memory, tinyxml2, Fast-CDR
set -e
echo "Installing Fast-DDS"

# Clone source code
if [ ! -d Fast-DDS ] ; then
  git clone https://github.com/eProsima/Fast-DDS.git
  cd Fast-DDS
else
  cd Fast-DDS
fi

# Use specific version
git fetch -p
git checkout v2.6.10
git submodule update --init --recursive

# Configure CMake
mkdir -p build && cd build
cmake .. $SHARED_CMAKE_ARGS \
         -DTHIRDPARTY_Asio=ON \
         -DCOMPILE_EXAMPLES=OFF \
         -DSQLITE3_SUPPORT=OFF

# Build and install
cmake --build . --target install --config Release -j $NUM_JOBS

echo "Installed Fast-DDS"
