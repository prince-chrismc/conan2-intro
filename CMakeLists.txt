cmake_minimum_required(VERSION 3.26)
project(app CXX)

find_package(spdlog CONFIG REQUIRED)
# For our particles
find_package(glfw3 REQUIRED CONFIG)
find_package(glad REQUIRED CONFIG)
find_package(linmath.h REQUIRED CONFIG)
find_package(tinycthread REQUIRED CONFIG)

add_executable(app src/main.cpp src/particles.hpp)

target_compile_features(app PRIVATE c_std_11)
target_link_libraries(app PRIVATE spdlog::spdlog glfw glad::glad
                                  linmath.h::linmath.h tinycthread::tinycthread)
