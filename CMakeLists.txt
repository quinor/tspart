cmake_minimum_required(VERSION 3.4)
project(tspart)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/deps)

#set(SFML_STATIC_LIBRARIES TRUE) # set this for static builds, you must have compiled SFML as static though

find_package(SFML 2.5 COMPONENTS graphics window REQUIRED)
find_package(TGUI 0.8 REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++14")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,-rpath=libs")


include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/deps/include)
link_directories(${CMAKE_SOURCE_DIR}/deps/lib)

file(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/*.cc src/*.cpp)
file(GLOB_RECURSE tools ${CMAKE_SOURCE_DIR}/tools/*.cc)

add_library(tspart ${sources})

set(libraries tspart tgui sfml-graphics sfml-window sfml-system)


foreach(tool_src ${tools})
    get_filename_component(binary ${tool_src} NAME_WE)
    add_executable(${binary} ${tool_src})
    target_link_libraries(${binary} tspart tgui sfml-graphics sfml-window sfml-system)
endforeach()
