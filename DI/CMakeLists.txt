cmake_minimum_required(VERSION 3.18)
project(Injecttor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(Injecttor INTERFACE
        Container.hpp
        Container.hpp)

target_include_directories(Injecttor INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})