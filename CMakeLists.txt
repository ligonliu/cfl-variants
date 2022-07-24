cmake_minimum_required(VERSION 3.13)

project(cfl_variants)

set(CMAKE_CXX_STANDARD 20)

add_executable(multicfl multicfl_main.cpp MultiCFL.cpp graph.cpp fileio0.cpp randomInterferenceGraph.cpp ColoringSaveLoad.cpp StringTokenizer.cpp ParameterOptimize.cpp)

add_executable(variablecfl variable_main.cpp VariableCFL.cpp IntColoringSaveLoad.cpp fileio0.cpp graph.cpp)
