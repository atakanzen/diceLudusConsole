cmake_minimum_required(VERSION 3.21)
project(diceLudusConsole)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-static-libgcc -static-libstdc++ -static")

add_executable(diceLudusConsole
        main.cpp)
