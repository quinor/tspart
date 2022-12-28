cmake_minimum_required(VERSION 3.4)
project(tspart)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(deps)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /O3")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")
endif()

set(CMAKE_INSTALL_RPATH "./lib")

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/*.cc ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE tools ${CMAKE_SOURCE_DIR}/tools/*.cc)
file(GLOB_RECURSE tests ${CMAKE_SOURCE_DIR}/tests/*.cc)

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

foreach(test_src ${tests})
    get_filename_component(binary ${test_src} NAME_WE)
    file(RELATIVE_PATH local_path ${CMAKE_SOURCE_DIR}/tests/ ${test_src})
    get_filename_component(local_dir ${local_path} DIRECTORY)
    add_executable(${binary} ${test_src})
    set_target_properties(${binary} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tests_bin/${local_dir}
    )
    target_link_libraries(${binary} ${libraries})
endforeach()


install(TARGETS sfml-graphics sfml-window sfml-system tspart tgui tspart_gui tesselation_gui
        RUNTIME DESTINATION . COMPONENT tspart_release
        LIBRARY DESTINATION lib COMPONENT tspart_release
        ARCHIVE DESTINATION lib/tspart COMPONENT tspart_release
)

install(DIRECTORY shaders misc
        DESTINATION . COMPONENT tspart_release)

install(FILES res/klaudia.jpg
        DESTINATION res/ COMPONENT tspart_release)
