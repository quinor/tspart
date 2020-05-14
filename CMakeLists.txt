cmake_minimum_required(VERSION 3.4)
project(tspart)

add_subdirectory(deps)

#find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
#find_package(TGUI 0.8 REQUIRED)


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -std=c++17 ${MAGICK_CXX_FLAGS}")
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
