cmake_minimum_required(VERSION 3.1)

# set the project name
set(project_name "CppToolKit-Factory")
set(example_name "example")
set(lib_name "factory")
set(test_name "test")

project(${project_name})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(SRC "${CMAKE_CURRENT_LIST_DIR}/src")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# subfolders
add_subdirectory("src")

add_subdirectory("test")

add_subdirectory("example")
