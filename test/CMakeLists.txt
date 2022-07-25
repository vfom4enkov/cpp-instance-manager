cmake_minimum_required(VERSION 3.0)

include_directories(${SRC})

file(GLOB MOCK_SOURCES "mock/*.cpp")
file(GLOB MOCK_HEADERS "mock/*.h")
file(GLOB TEST_HEADERS "*.h")
file(GLOB TEST_SOURCES "*.cpp")

add_executable(${test_name}
                ${MOCK_SOURCES}
                ${MOCK_HEADERS}
                ${TEST_HEADERS}
                ${TEST_SOURCES})

source_group("mock" FILES ${MOCK_SOURCES} ${MOCK_HEADERS})

FIND_PACKAGE(Boost)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()
