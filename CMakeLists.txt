cmake_minimum_required(VERSION 2.8)

option(test "Build all tests." OFF)

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

################################
# Testing
# copied from https://github.com/dmonopoly/gtest-cmake-example/blob/master/CMakeLists.txt
################################
if (test)
  include_directories(test/mocks)

  # This adds another subdirectory, which has 'project(gtest)'.
  add_subdirectory(submodules/googletest/googlemock)

  enable_testing()

  # Include the gtest library. gtest_SOURCE_DIR is available due to
  # 'project(gtest)' above.
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

  ##############
  # Unit Tests
  ##############
  file(GLOB TEST_SOURCES "test/*.cpp")
  add_executable(runUnitTests ${TEST_SOURCES})

  # Standard linking to gtest stuff.
  target_link_libraries(runUnitTests gtest gtest_main)

  # Extra linking for the project.
  target_link_libraries(runUnitTests libbixer)

  # This is so you can do 'make test' to see all your tests run, instead of
  # manually running the executable runUnitTests to see those specific tests.
  add_test(NAME that-test-I-made COMMAND runUnitTests)

  # You can also omit NAME and COMMAND. The second argument could be some other
  # test executable.
  add_test(that-other-test-I-made runUnitTests)
endif()