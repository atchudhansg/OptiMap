cmake_minimum_required(VERSION 3.10)
project(pPIMCompiler)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

add_executable(pPIMCompiler
    src/main.cpp
    src/IRInstruction.cpp
    src/StrategyGenerator.cpp
    src/CostEstimator.cpp
)

target_include_directories(pPIMCompiler PRIVATE include)
