#include "IRInstruction.hpp"
#include "StrategyGenerator.hpp"
#include "CostEstimator.hpp"
#include "CompilerMemory.hpp"
#include "MemoryMapper.hpp"
#include "LLVMFrontend.hpp"
#include "HHESS.hpp"

#include <iostream>
#include <iomanip>

int main() {
    std::cout << "\n========== LLVM IR Input Analysis ==========\n";
    LLVMFrontend::parseIRFile("../matrix_mul.ll");

    // Matrix dimensions
    const int M = 2, N = 2, K = 2;
    std::string matrix_size = std::to_string(M) + "x" + std::to_string(N);

    StrategyGenerator gen(M, N, K);
    MemoryMapper memoryMapper;
    HHESS strategySelector("compiler_memory.json"); // Uses same file

    // Step 1: Select best strategy from HHESS
    std::string chosenStrategy = strategySelector.selectBestStrategy(matrix_size);
    std::cout << "\n✅ Chosen Strategy from HHESS: " << chosenStrategy << "\n";

    // Step 2: Generate IR
    StrategyType type;
    if (chosenStrategy == "ijk") type = StrategyType::IJK;
    else if (chosenStrategy == "ikj") type = StrategyType::IKJ;
    else type = StrategyType::TILED_4x4;

    auto irList = gen.generate(type);

    std::cout << "\nStrategy: " << gen.strategyName(type) << "\n";
    for (const auto& instr : irList) {
        std::cout << instr.toString() << " | 0x"
                  << std::hex << std::setw(6) << std::setfill('0') 
                  << instr.encodeToISA() << std::dec << "\n";

        int actualRow = instr.getRow();
        auto mapping = memoryMapper.toString(0, actualRow, 0);
        std::cout << "    ↳ " << mapping << "\n";
    }

    // Step 3: Estimate cost
    CostEstimator estimator;
    int cost = estimator.estimateCost(irList);
    std::cout << "Estimated Cost: " << cost << "\n";

    // Step 4: Log and evolve via HHESS
    strategySelector.recordResult(matrix_size, gen.strategyName(type), cost);

    return 0;
}
