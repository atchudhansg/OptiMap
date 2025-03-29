cmake_minimum_required(VERSION 3.10)
project(pPIMCompiler)

set(CMAKE_CXX_STANDARD 17)

# 1) Include path for the nlohmann JSON if needed:
include_directories(include/nlohmann/json/include)

# 2) Add your executable and list only the .cpp files that actually exist:
add_executable(pPIMCompiler
    src/main.cpp
    src/IRInstruction.cpp
    src/StrategyGenerator.cpp
    src/CostEstimator.cpp
    src/CompilerMemory.cpp
    src/MemoryMapper.cpp
    src/LLVMFrontend.cpp
    src/HHESS.cpp
    src/NSS.cpp
    # Remove or comment out src/MLModel.cpp if it doesn't exist
    src/MLPModel.cpp
)

# 3) Explicitly add your "include" directory to the target's include paths
target_include_directories(pPIMCompiler PRIVATE include)
