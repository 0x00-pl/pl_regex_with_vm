cmake_minimum_required(VERSION 2.6)
project(plre)
SET(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Wextra")


#install(TARGETS plre RUNTIME DESTINATION bin)

add_subdirectory(src)

add_executable(plre 
src/ast.cpp 
src/il.cpp
src/regex.cpp
src/str_convert.cpp
src/vm.cpp
test.cpp 
)