cmake_minimum_required(VERSION 3.10)

# set the project name
project(CppInstanceManager)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# subfolders
add_subdirectory("src")