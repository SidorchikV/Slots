cmake_minimum_required(VERSION 3.2.3) 
include(config.cmake)
include(build.cmake)

project(Slots)

#set (SOURCE src/slots.cpp )


#add_executable(Slots ${SOURCE}) 


include_directories(lib)

set(SOURCE_EXE test/test.cpp)

set(SOURCE_LIB lib/slots.cpp)
add_library(slots STATIC ${SOURCE_LIB})

add_executable(test ${SOURCE_EXE})

target_link_libraries(test slots)