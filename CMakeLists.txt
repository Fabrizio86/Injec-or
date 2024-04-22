cmake_minimum_required(VERSION 3.0)
project(InjecttorTest)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(DI)

add_executable(InjecttorTest main.cpp
        main.cpp)
target_link_libraries(InjecttorTest PRIVATE Injecttor)