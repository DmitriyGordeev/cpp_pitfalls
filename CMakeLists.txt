cmake_minimum_required(VERSION 3.25)
project(UsefullCpp)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(std_variant/)
add_subdirectory(std_forward/)

add_executable(FuncPointers function_pointers.cpp)
add_executable(Functional stdfunc.cpp)
add_executable(Functors functors.cpp)
add_executable(ImplicitConversion implicit_conversion.cpp)
add_executable(Constcast constcast.cpp)
