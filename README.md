# TSPART

## Requirements

You have to provide:
  * make
  * cmake
  * glm
  * libmagick-7
  * C++17-compatible compiler (preferably clang)
  * SFML build dependencies (*installing dependencies* list from [here](https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php))


## Build instructions

Run `make` to download/compile other dependencies and build the project automatically. The project uses CMake and it might be better to use it to build manually, the `Makefile` is not very robust.

The binaries will be built and copied to the project's main directory.


## Usage instructions

Run the `./tspart_gui` binary and play with the sliders. "Preview" shows preprocessed image and "Draw!" launches the drawing process and saves the output svg to the "Output file". `output.gcode` is also generated after each run - it contains a very simple gcode generated directly from the TSP path.
