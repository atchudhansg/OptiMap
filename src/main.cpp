#include "IRInstruction.hpp"
#include "StrategyGenerator.hpp"
#include <iostream>
#include <iomanip>
#include "CostEstimator.hpp"

int main() {
    StrategyGenerator gen(2, 2, 2);

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
    }
    

    return 0;
}
