cmake_minimum_required(VERSION 3.4)

set(CMAKE_OSX_ARCHITECTURES "x86_64")

project(ZipImage)
set(CMAKE_PREFIX_PATH "/usr/local/opt/qt@5")
find_package(Imagine REQUIRED COMPONENTS Graphics)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(quadtree)

add_executable(zipimage quadtree/quadtree.h zipimage.cpp main.cpp)
target_link_libraries(zipimage Imagine::Graphics)
