cmake_minimum_required(VERSION 4.1)
project(IZBRANI_ALGORITMI_KOMBINATORIKE)

set(CMAKE_CXX_STANDARD 20)

add_executable(IZBRANI_ALGORITMI_KOMBINATORIKE main.cpp
        classes/RestrictionCarting.cpp
        classes/RestrictionCarting.h
        classes/Util.cpp
        classes/Util.h
)
