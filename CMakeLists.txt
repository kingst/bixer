cmake_minimum_required(VERSION 2.8)
project(bixer)
find_package (Threads)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(src)
file(GLOB SOURCES "src/*.cpp" "src/*.c")

add_executable(bixer ${SOURCES})
target_link_libraries (bixer ${CMAKE_THREAD_LIBS_INIT})