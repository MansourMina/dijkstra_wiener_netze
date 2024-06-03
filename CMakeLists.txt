cmake_minimum_required(VERSION 3.28)
project(PB3)

set(CMAKE_CXX_STANDARD 17)

add_executable(PB3 main.cpp
        Station.cpp
        Station.h
        Network.cpp
        Network.h
        Line.cpp
        Line.h
)
