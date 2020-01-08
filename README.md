# PiGPGPU
 This is a simple CMake project using [QPULib](https://github.com/m516/QPULib-CMake). It also serves as a template for future projects
 
## Try me!

This code has been tested on devices running Ubuntu, Windows 10, and Raspbian Buster. You can use it too:

### Windows

Open this project as a CMake project in Visual Studio or Visual Studio Code. CMake comes with the *Desktop Development with C++* workload in Visual Studio, but you will have to install CMake from [its website](https://cmake.org/download/) an get the CMake extension in Visual Studio Code.

To build the project, use the build button in Visual Studio, or use the CMake commands or CMake GUI, then make.

### Ubuntu and Raspbian (and other Linux-based systems)

You will need C++, Make, and CMake. Install them with your package manager. In Ubuntu and Raspbian, install them with apt:

```
sudo apt-get install g++ make cmake
```

1. Then navigate to your project directory: `cd <path_to_project_root_folder>/`

2. Generate Makefiles from the CMake scripts: `cmake .`

3. Build the project with the Makefiles: `make`
