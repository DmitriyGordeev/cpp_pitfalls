cmake_minimum_required(VERSION 3.25)
project(CppThreads)

set(CMAKE_CXX_STANDARD 17)

add_executable(FuncPointers function_pointers.cpp)
add_executable(Functional stdfunc.cpp)
add_executable(Functors functors.cpp)