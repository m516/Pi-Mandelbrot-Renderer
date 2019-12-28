# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("PiGPGPU")

# Include sub-projects.
add_subdirectory ("src")

add_custom_command(OUTPUT qpulib
                   COMMAND make all
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/QPULib
)