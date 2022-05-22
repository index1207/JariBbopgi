cmake_minimum_required(VERSION 3.21)
project(sfml)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_FLAGS "-fexec-charset=CP949")
set(CMAKE_CXX_FLAGS "-static -static-libgcc -static-libstdc++")

# SFML
## If you want to link SFML statically
set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR ${PROJECT_SOURCE_DIR}/libs/SFML-2.5.1/lib/cmake/SFML)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/libs/SFML-2.5.1/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
add_executable(jaribbopgi main.cpp)
target_link_libraries(jaribbopgi sfml-system sfml-window sfml-graphics sfml-network sfml-audio)