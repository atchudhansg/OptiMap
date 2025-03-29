#include "IRInstruction.hpp"
#include "StrategyGenerator.hpp"
#include "CostEstimator.hpp"
#include "CompilerMemory.hpp"
#include <iostream>
#include <iomanip>

int main() {
    StrategyGenerator gen(2, 2, 2);
    CompilerMemory compilerMemory("compiler_memory.json");
    compilerMemory.load();

    for (StrategyType type : {StrategyType::IJK, StrategyType::IKJ, StrategyType::TILED_4x4}) {
        std::cout << "\nStrategy: " << gen.strategyName(type) << "\n";
    
        auto irList = gen.generate(type);
        
        // Print IR
        for (const auto& instr : irList) {
            std::cout << instr.toString() << " | 0x"
                      << std::hex << std::setw(6) << std::setfill('0') 
                      << instr.encodeToISA() << std::dec << "\n";
        }
    
        // Estimate cost (once per strategy)
        CostEstimator estimator;
        int cost = estimator.estimateCost(irList);
        std::cout << "Estimated Cost: " << cost << "\n";

        // Log the strategy and cost to memory
        std::string matrix_size = std::to_string(2) + "x" + std::to_string(2); // Use dynamic size if needed
        compilerMemory.log(matrix_size, gen.strategyName(type), cost);
    }

    // Select best strategy based on history
    std::string bestStrategy = compilerMemory.getBestStrategy("2x2");
    if (!bestStrategy.empty()) {
        std::cout << "Best Strategy based on history: " << bestStrategy << "\n";
    } else {
        std::cout << "No historical data available for this matrix size.\n";
    }

    return 0;
}
