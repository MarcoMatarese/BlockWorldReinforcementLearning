cmake_minimum_required(VERSION 3.14)
project(BlockWorldRL)

set(CMAKE_CXX_STANDARD 11)

add_executable(BlockWorldRL main.cpp BlockWorldReinforcementLearner.cpp BlockWorldReinforcementLearner.h BlockWorldEnvironment.cpp BlockWorldEnvironment.h)