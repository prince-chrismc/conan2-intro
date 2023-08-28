cmake_minimum_required(VERSION 3.26)
project(app CXX)

find_package(spdlog CONFIG REQUIRED)
# We’ll add more shortly

add_executable(app src/main.cpp)

target_link_libraries(app PRIVATE spdlog::spdlog)
