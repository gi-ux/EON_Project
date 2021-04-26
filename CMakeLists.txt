cmake_minimum_required(VERSION 3.17)
project(EON_Project)

set(CMAKE_CXX_STANDARD 14)

add_executable(EON_Project src/main.cpp src/Path.cpp src/Path.h src/Link.cpp src/Link.h src/Data.cpp src/NDArray.cpp src/Demand.cpp src/Demand.h src/PathsForT.cpp src/PathsForT.h src/Data.h src/Parser.cpp src/HeuristicGreedy.cpp)