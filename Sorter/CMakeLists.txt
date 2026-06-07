cmake_minimum_required(VERSION 3.10)

project(Sorter LANGUAGES CXX)

find_package(SFML REQUIRED COMPONENTS graphics window system)

add_executable(Sorter main.cpp)

target_link_libraries(Sorter
    sfml-graphics
    sfml-window
    sfml-system
)