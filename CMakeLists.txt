cmake_minimum_required(VERSION 2.8)
project(bixer)
find_package (Threads)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(src/lib/include src/include)
file(GLOB LIB_SOURCES "src/lib/*.cpp" "src/lib/*.c")

add_library(libbixer ${LIB_SOURCES})

add_executable(bixer src/bixer.cpp src/IptvService.cpp)
target_link_libraries (bixer ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (bixer libbixer)