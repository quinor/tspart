# TSPART

## Requirements

You have to provide:
  * make
  * cmake
  * C++17-compatible compiler
  * SFML build dependencies (*installing dependencies* list from [here](https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php))


## Build instructions

Run `cmake -B build` and then `cmake --build build -j4` to build the project.

The binaries will be built and copied to the project's main directory.


## Usage instructions

Run the `./tspart_gui` binary and play with the sliders. "Preview" shows preprocessed image and "Draw!" launches the drawing process and saves the output svg to the "Output file". `output.gcode` is also generated after each run - it contains a very simple gcode generated directly from the TSP path.

Run the `./tesselation_gui` for an experimental application for generating tesselation art. The gui is very similar to the `tspart_gui`, but the slider values are significantly less robust - they should work well though for the default parameters.
