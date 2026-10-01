cmake_minimum_required(VERSION 3.15)
project(SoftwareRenderer)

set(CMAKE_CXX_STANDARD 17)

# looking for SDL2 in system
find_package(SDL2 REQUIRED)

add_executable(SoftwareRenderer 
    src/main.cpp
)

target_link_libraries(SoftwareRenderer PRIVATE SDL2::SDL2)