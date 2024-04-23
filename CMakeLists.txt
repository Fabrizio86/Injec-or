cmake_minimum_required(VERSION 3.18)
project(InjecttorTest)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(DI)

add_executable(InjecttorTest main.cpp
        main.cpp
        Examples/SimpleExample.h
        Examples/SubDependencyExample.h
        Examples/AdvancedExample.h
        Examples/WebExample.h
        Examples/AdvancedWebExample.h)
target_link_libraries(InjecttorTest PRIVATE Injecttor)