cmake_minimum_required(VERSION 3.4)
project(tspart)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/deps)

#set(SFML_STATIC_LIBRARIES TRUE) # set this for static builds, you must have compiled SFML as static though


find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(TGUI 0.8 REQUIRED)


find_program(MAGICK_CONFIG "Magick++-config")
# Ask about CXX and lib flags/locations
execute_process(COMMAND "${MAGICK_CONFIG}" "--cxxflags" OUTPUT_VARIABLE MAGICK_CXX_FLAGS)
execute_process(COMMAND "${MAGICK_CONFIG}" "--libs" OUTPUT_VARIABLE MAGICK_LD_FLAGS)
# Remove trailing whitespace (CMAKE warns about this)
string(STRIP "${MAGICK_CXX_FLAGS}" MAGICK_CXX_FLAGS)
string(STRIP "${MAGICK_LD_FLAGS}" MAGICK_LD_FLAGS)


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -std=c++17 ${MAGICK_CXX_FLAGS} ${MAGICK_LD_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,-rpath=libs")


include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/*.cc src/*.cpp)
file(GLOB_RECURSE tools ${CMAKE_SOURCE_DIR}/tools/*.cc)

add_library(tspart ${sources})
target_link_libraries(tspart sfml-graphics sfml-window sfml-system)

set(libraries tspart tgui)


foreach(tool_src ${tools})
    get_filename_component(binary ${tool_src} NAME_WE)
    file(RELATIVE_PATH local_path ${CMAKE_SOURCE_DIR}/tools/ ${tool_src})
    get_filename_component(local_dir ${local_path} DIRECTORY)
    add_executable(${binary} ${tool_src})
    set_target_properties(${binary} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${local_dir}
    )
    target_link_libraries(${binary} ${libraries})
endforeach()
