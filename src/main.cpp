#include "IRInstruction.hpp"
#include "StrategyGenerator.hpp"
#include "CostEstimator.hpp"
#include "CompilerMemory.hpp"
#include "MemoryMapper.hpp"
#include "LLVMFrontend.hpp"

#include <iostream>
#include <iomanip>

int main() {
    // Step 1: LLVM IR Frontend
    std::cout << "\n========== LLVM IR Input Analysis ==========\n";
    LLVMFrontend::parseIRFile("../matrix_mul.ll");

    // Step 2: Matrix multiply configuration
    const int M = 2, N = 2, K = 2;
    StrategyGenerator gen(M, N, K);
    CompilerMemory compilerMemory("compiler_memory.json");
    compilerMemory.load();

    // Step 3: Memory mapping simulation
    MemoryMapper memoryMapper;

    for (StrategyType type : {StrategyType::IJK, StrategyType::IKJ, StrategyType::TILED_4x4}) {
        std::cout << "\nStrategy: " << gen.strategyName(type) << "\n";
        auto irList = gen.generate(type);

        // Print IR and show memory mapping
        for (const auto& instr : irList) {
            std::cout << instr.toString() << " | 0x"
                      << std::hex << std::setw(6) << std::setfill('0') 
                      << instr.encodeToISA() << std::dec << "\n";

            // ✅ Use actual row from encoded instruction
            int actualRow = instr.getRow();
            auto mapping = memoryMapper.toString(0, actualRow, 0);  // matrixId=0, col=0 for now
            std::cout << "    ↳ " << mapping << "\n";
        }

        // Estimate cost (once per strategy)
        CostEstimator estimator;
        int cost = estimator.estimateCost(irList);
        std::cout << "Estimated Cost: " << cost << "\n";

        // Log the strategy and cost to memory
        std::string matrix_size = std::to_string(M) + "x" + std::to_string(N);
        compilerMemory.log(matrix_size, gen.strategyName(type), cost);
    }

    // Best strategy based on history
    std::string bestStrategy = compilerMemory.getBestStrategy("2x2");
    if (!bestStrategy.empty()) {
        std::cout << "Best Strategy based on history: " << bestStrategy << "\n";
    } else {
        std::cout << "No historical data available for this matrix size.\n";
    }

    return 0;
}
