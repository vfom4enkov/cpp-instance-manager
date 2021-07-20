cmake_minimum_required(VERSION 3.10)

set(instance_manager "instance_manager")

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_library(${instance_manager} ${SOURCES} ${HEADERS})

set_target_properties(instance_manager PROPERTIES LINKER_LANGUAGE CXX)
