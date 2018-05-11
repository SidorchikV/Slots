cmake_minimum_required(VERSION 3.2.3) 
include(config.cmake)
include(build.cmake)

project(Slots)
include_directories(lib)

set(LIB_SRC lib/slots.cpp)
set(LIB_HDR lib/slots.h)

SOURCE_GROUP("Header Files" FILES ${LIB_HDR})
SOURCE_GROUP("Source Files" FILES ${LIB_SRC})

add_library(slots STATIC ${LIB_SRC})

set(TEST_SRC test/test_slots.cpp)
add_executable(test_slots ${TEST_SRC})

target_link_libraries(test_slots slots)