cmake_minimum_required(VERSION 3.1)

file(GLOB HEADERS "cpptoolkit/factory/*.h")
file(GLOB MANAGER_HEADERS "cpptoolkit/factory/manager/*.h")
file(GLOB CONTEXT_HEADERS "cpptoolkit/factory/manager/context/*.h")

add_library(${lib_name} ${HEADERS} ${MANAGER_HEADERS} ${CONTEXT_HEADERS})

source_group("Header Files/manager" FILES ${MANAGER_HEADERS})
source_group("Header Files/manager/context" FILES ${CONTEXT_HEADERS})

set_target_properties(${lib_name} PROPERTIES LINKER_LANGUAGE CXX)
