
cmake_minimum_required(VERSION 3.16) 

# Spécifiez l'architecture cible
set(CMAKE_OSX_ARCHITECTURES "x86_64")

# Définissez les compilateurs explicitement
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

# Déclarez le projet et le langage
project(villes LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "/usr/local/opt/qt@5")

# Recherchez la bibliothèque Imagine
find_package(Imagine REQUIRED COMPONENTS Graphics)

# Définir le standard C++
set(CMAKE_CXX_STANDARD 11)


add_executable(example example.cpp town.cpp town.h point2d.cpp point2d.h)
add_executable(isotopo isotopo.cpp town.cpp town.h point2d.cpp point2d.h)

add_definitions(-DsrcPath="${CMAKE_CURRENT_SOURCE_DIR}")



