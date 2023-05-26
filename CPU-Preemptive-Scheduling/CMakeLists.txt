cmake_minimum_required(VERSION 3.5)

project(CPU-Preemptive-Scheduling LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CPU-Preemptive-Scheduling main.cpp program.h program.cpp)

install(TARGETS CPU-Preemptive-Scheduling
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
