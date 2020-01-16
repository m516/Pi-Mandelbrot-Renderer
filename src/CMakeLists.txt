# CMakeList.txt : CMake project for PiGPGPU, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(PiGPGPU)

# Local header files here ONLY
SET(TARGET_HEADERS
   PiGPGPU.h
   )

# Local source files here
SET(TARGET_SOURCES
    PiGPGPU.cpp
    )

    
add_definitions(-fpermissive)

# Add source to this project's executable.
add_executable (PiGPGPU
${TARGET_SOURCES} 
${TARGET_HEADERS} 
)

#Link project with QPULib
target_link_libraries(PiGPGPU QPULib)

#Link project with SFML
add_dependencies(PiGPGPU SFML)
#target_link_libraries(PiGPGPU sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
target_link_libraries(${SFML_LIBRARIES})