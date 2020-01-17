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

	#Set SFML root directory
	set(SFML_ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml-pi)

else() #Not compiling on the Pi
	message(STATUS "This is not a Pi")
	message(STATUS "Emulating Pi's GPU")
	#Add precompiler definitions
	add_definitions(-DEMULATION_MODE)

	#Set SFML root directory
	set(SFML_ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml)
endif()

#Include QPULib
add_subdirectory("lib/QPULib")
include_directories("${PROJECT_SOURCE_DIR}/lib/QPULib/Lib")

#Include SFML
set(SFML_STATIC_LIBRARIES TRUE)
set(BUILD_SHARED_LIBS FALSE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml)

add_subdirectory("src")