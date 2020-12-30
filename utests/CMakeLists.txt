cmake_minimum_required(VERSION 3.10)

add_executable(utests utests_math.cpp)
target_link_libraries(utests PRIVATE doctest kuro)