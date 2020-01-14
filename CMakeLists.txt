# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (PiGPGPU)

# Include libraries
add_subdirectory("lib")


#Compile differently for the Raspberry Pi
find_path(HAS_BCMHOST bcm_host.h PATHS /opt/vc/include/)
if (HAS_BCMHOST) #This is a Pi
	message(STATUS "This is a Pi")
	message(STATUS "Targeting VideoCore IV GPU")
	#Set precompiler flags
	add_definitions(-DQPU_MODE)

	#Include sfml-pi
	add_subdirectory(lib/sfml-pi)
	include_directories("${PROJECT_SOURCE_DIR}/lib/sfml-pi/include/SFML")

else() #Not compiling on the Pi
	message(STATUS "This is not a Pi")
	message(STATUS "Emulating Pi's GPU")
	#Add precompiler definitions
	add_definitions(-DEMULATION_MODE)

	#Include sfml
	add_subdirectory(lib/sfml)
	include_directories("${PROJECT_SOURCE_DIR}/lib/sfml/include/SFML")
endif()

#Include QPULib
add_subdirectory("lib/QPULib")
include_directories("${PROJECT_SOURCE_DIR}/lib/QPULib/Lib")

add_subdirectory("src")