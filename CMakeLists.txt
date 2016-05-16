cmake_minimum_required(VERSION 2.6)
project(call_benchmark)

add_executable(call_benchmark main.cpp)

install(TARGETS call_benchmark RUNTIME DESTINATION bin)
