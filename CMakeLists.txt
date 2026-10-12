cmake_minimum_required(VERSION 3.22.1)

# ===================================================================
#      PROJECT SETUP
# ===================================================================
project(flexiv_sim_plugin VERSION 2.1.0 LANGUAGES CXX)

# Configure build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "CMake build type" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release" "Debug" "RelWithDebInfo" "MinSizeRel")

# Select the prebuilt library for this platform. Sim Plugin is shipped as a self-contained shared
# library.
message(STATUS "OS: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
    set(SIM_PLUGIN_LIB "libflexiv_sim_plugin.x86_64-linux-gnu.so")
  else()
    message(FATAL_ERROR "Linux with ${CMAKE_SYSTEM_PROCESSOR} processor is currently not supported.")
  endif()
else()
  message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} is currently not supported.")
endif()

# Download a release asset by file name, verifying its SHA256 against the
# checked-in <name>.sha256 placeholder.
function(sim_plugin_download_release_asset asset_name)
  file(READ ${CMAKE_CURRENT_SOURCE_DIR}/lib/${asset_name}.sha256 expected_hash)
  set(asset_url https://github.com/flexivrobotics/flexiv_sim_plugin/releases/download/v${PROJECT_VERSION}/${asset_name})
  message(STATUS "Downloading ${asset_url} ...")
  file(DOWNLOAD ${asset_url} ${CMAKE_CURRENT_BINARY_DIR}/${asset_name}
       SHOW_PROGRESS
       TLS_VERIFY ON
       EXPECTED_HASH SHA256=${expected_hash}
       STATUS status)
  list(GET status 0 code)
  if(NOT code EQUAL 0)
    message(FATAL_ERROR "Download failed: ${status}")
  endif()
  message(STATUS "Download finished: ${asset_name}")
endfunction()

sim_plugin_download_release_asset(${SIM_PLUGIN_LIB})

# ===================================================================
#      PROJECT DEPENDENCIES
# ===================================================================
# Threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
message(STATUS "Found Threads")

# ===================================================================
#      CREATE LIBRARY
# ===================================================================
# Create the library target with a dummy source. The dummy output is replaced at
# install time by the downloaded prebuilt shared library (see FlexivInstallLibrary).
add_library(${PROJECT_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/lib/dummy.cpp)

# Create an alias of the target with flexiv namespace
add_library(flexiv::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Set include directories
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Link to dependencies. Every thirdparty dependency (Fast-DDS, spdlog, ...) is embedded in the prebuilt
# library and hidden, so only Threads leaks through the public API.
target_link_libraries(${PROJECT_NAME} PUBLIC
  Threads::Threads
)

# Use moderate compiler warning option
if(CMAKE_HOST_UNIX)
  target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra)
else()
  target_compile_options(${PROJECT_NAME} PUBLIC /W1)
endif()

# ===================================================================
#      LIBRARY INSTALLATION
# ===================================================================
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FlexivInstallLibrary.cmake)
FlexivInstallLibrary()
