cmake_minimum_required(VERSION 3.29)
project(cpp_examples)

add_library(dll_producer SHARED LibClass.cpp)

#target_compile_definitions(dll_example PRIVATE DLLSPEC)

add_executable(dll_consumer main.cpp)
target_link_libraries(dll_consumer dll_producer)

add_executable(runtime_load runtime_load.cpp)
