cmake_minimum_required(VERSION 3.16)

# Add factory to #include search path
include_directories(${SRC})

file(GLOB HEADERS "*.h")
file(GLOB SOURCES "*.cpp")

# add the executable
add_executable(${example_name} ${HEADERS} ${SOURCES})

# Specify examples's dependency on factory.
#target_link_libraries(${example_name} ${lib_name})
