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
	
	#Configure SFML
	add_subdirectory(lib/sfml-pi)
	set(${PROJECT_SOURCE_DIR}/lib/sfml-pi)

else() #Not compiling on the Pi
	message(STATUS "This is not a Pi")
	message(STATUS "Emulating Pi's GPU")
	#Add precompiler definitions
	add_definitions(-DEMULATION_MODE)

	#Configure SFML
	set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml/cmake/SFML)

endif()

#Include QPULib
add_subdirectory("lib/QPULib")
include_directories("${PROJECT_SOURCE_DIR}/lib/QPULib/Lib")
#Include SFML
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml/include)
#set(BUILD_SHARED_LIBS FALSE)
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS window system graphics)

add_subdirectory("src")