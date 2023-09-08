cmake_minimum_required(VERSION 3.26)
project(app CXX)

find_package(spdlog CONFIG REQUIRED)
# For our particles
find_package(glfw3 REQUIRED CONFIG)
find_package(glad REQUIRED CONFIG)
find_package(linmath.h REQUIRED CONFIG)
find_package(tinycthread REQUIRED CONFIG)

add_library(particles INTERFACE src/particles.hpp)
target_include_directories(particles INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
# Because we are header-only we need to explicitly tell CMake
set_target_properties(particles PROPERTIES LINKER_LANGUAGE CXX)

add_executable(app src/main.cpp)
target_compile_features(app PRIVATE c_std_11)
target_link_libraries(app PRIVATE particles spdlog::spdlog glfw glad::glad
                                  linmath.h::linmath.h tinycthread::tinycthread)

include(CTest)
if(BUILD_TESTING)
  find_package(GTest REQUIRED CONFIG)
  include(GoogleTest)

  add_executable(particle-tests tests/particles.cpp)
  target_link_libraries(particle-tests PRIVATE particles GTest::gtest
                                               GTest::gtest_main)
  gtest_add_tests(TARGET particle-tests)
endif()
