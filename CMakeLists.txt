cmake_minimum_required(VERSION 2.6)
project(plre)

add_executable(plre main.cpp)

install(TARGETS plre RUNTIME DESTINATION bin)
