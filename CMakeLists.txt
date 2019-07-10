cmake_minimum_required(VERSION 3.14)
project(Robot_Governance_Project)

set(CMAKE_CXX_STANDARD 14)

add_executable(Robot_Governance_Project main.cpp Action.cpp Action.h Predicate.cpp Predicate.h Environment.cpp Environment.h Agent.cpp Agent.h)