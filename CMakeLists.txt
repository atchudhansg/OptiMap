cmake_minimum_required(VERSION 3.10)
project(pPIMCompiler)

set(CMAKE_CXX_STANDARD 17)

# Include path corrected to point directly to the nested include directory
include_directories(include/nlohmann/json/include)

# Add your executable
add_executable(pPIMCompiler
    src/main.cpp
    src/IRInstruction.cpp
    src/StrategyGenerator.cpp
    src/CostEstimator.cpp
    src/CompilerMemory.cpp
    src/MemoryMapper.cpp
    src/LLVMFrontend.cpp
    src/HHESS.cpp
)


# Explicitly include headers from your project's "include" directory
target_include_directories(pPIMCompiler PRIVATE include)
