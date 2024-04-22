cmake_minimum_required(VERSION 3.0)
project(Injecttor)

set(CMAKE_CXX_STANDARD 17)

add_library(Injecttor INTERFACE
        Container.h
        Container.h)
target_include_directories(Injecttor INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})