cmake_minimum_required(VERSION 3.25)
project(CppHints)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(std_variant/)
add_subdirectory(std_forward/)
add_subdirectory(templates/)
add_subdirectory(memory/)
add_subdirectory(singleton/)

add_executable(FuncPointers function_pointers.cpp)
add_executable(Functional stdfunc.cpp)
add_executable(ImplicitConversion implicit_conversion.cpp)
add_executable(Constcast constcast.cpp)
add_executable(default_copy_constructor bad_example_default_copy_constructor.cpp)