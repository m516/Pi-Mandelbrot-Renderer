# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (PiGPGPU)

#Include modules
include(ExternalProject)

# Include libraries
add_subdirectory("lib")

#Compile differently for the Raspberry Pi
find_path(HAS_BCMHOST bcm_host.h PATHS /opt/vc/include/)
if (HAS_BCMHOST) #This is a Pi
	message(STATUS "This is a Pi")
	message(STATUS "Targeting VideoCore IV GPU")
	#Set precompiler flags
	add_definitions(-DQPU_MODE)

else() #Not compiling on the Pi
	message(STATUS "This is not a Pi")
	message(STATUS "Emulating Pi's GPU")
	#Add precompiler definitions
	add_definitions(-DEMULATION_MODE)
endif()

#Include QPULib
add_subdirectory("lib/QPULib")
include_directories("${PROJECT_SOURCE_DIR}/lib/QPULib/Lib")

#Include SFML
set(sfml_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/sfml")
set(sfml_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/sfml")
set(sfml_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${sfml_INSTALL_DIR} -DBUILD_SHARED_LIBS=FALSE)

ExternalProject_Add(SFML
	PREFIX ${sfml_PREFIX}
	SOURCE_DIR  ${CMAKE_SOURCE_DIR}/lib/sfml
	INSTALL_DIR ${sfml_INSTALL_DIR}
	CMAKE_ARGS ${sfml_CMAKE_ARGS}
)
#set(SFML_INCLUDE_DIR ${sfml_INSTALL_DIR}/include) #Otherwise this is filled by FindSFML.cmake.
#message(STATUS "CMAKE install prefix: ${CMAKE_INSTALL_PREFIX}")
set(SFML_LIBRARIES
	${sfml_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-audio${CMAKE_STATIC_LIBRARY_SUFFIX}
	${sfml_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-graphics${CMAKE_STATIC_LIBRARY_SUFFIX}
	${sfml_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-network${CMAKE_STATIC_LIBRARY_SUFFIX}
	${sfml_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-system${CMAKE_STATIC_LIBRARY_SUFFIX}
	${sfml_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-window${CMAKE_STATIC_LIBRARY_SUFFIX}
)


include_directories(${SFML_INCLUDE_DIR})
message(STATUS "SFML include directory: ${SFML_INCLUDE_DIR}")

message(STATUS "SFML library path: ${CMAKE_INSTALL_PREFIX}/lib/")

add_subdirectory("src")