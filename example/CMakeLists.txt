cmake_minimum_required(VERSION 3.1)

# Add factory to #include search path
include_directories(${SRC})

file(GLOB HEADERS "*.h")
file(GLOB SOURCES "*.cpp")

file(GLOB LOG_HEADERS "log/*.h")
file(GLOB LOG_SOURCES "log/*.cpp")

# add the executable
add_executable(${example_name} ${HEADERS} ${SOURCES} ${LOG_HEADERS} ${LOG_SOURCES})
