# TSPART

![example #2, tspart_gui](/readme_res/example_2.png)
## Download

The app can be downloaded from the releases - no need to build it by hand!


## Usage instructions

Run the `./tspart_gui` binary and play with the sliders. "Preview" shows preprocessed image and "Draw!" launches the drawing process and saves the output svg to the "Output file". `output.gcode` is also generated after each run - it contains a very simple gcode generated directly from the TSP path.

Run the `./tesselation_gui` for an experimental application for generating tesselation art. The gui is very similar to the `tspart_gui`, but the slider values are significantly less robust - they should work well though for the default parameters.


## Examples

![example #1, tspart_gui](/readme_res/example_1.png)

![example #2, tspart_gui](/readme_res/example_2.png)

![example #3, tspart_gui](/readme_res/example_3.png)

![example #4, tspart_gui](/readme_res/example_4.png)

![example #5, tspart_gui](/readme_res/example_5.png)

![example #6, tesselation_gui](/readme_res/example_6.jpg)

![example #7, tesselation_gui](/readme_res/example_7.jpg)

## For developent - build instructions

You have to provide:
  * make
  * cmake
  * C++17-compatible compiler
  * SFML build dependencies (*installing dependencies* list from [here](https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php))

Run `git submodule update --init` after cloning to update the submodule dependencies or clone the repository with `git clone https://github.com/quinor/tspart.git --recursive` to download the submodules instantly.

Run `cmake -B build .` and then `cmake --build build -j4` to build the project.

The binaries will be built and copied to the project's main directory.
