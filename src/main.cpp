#include "IRInstruction.hpp"
#include "StrategyGenerator.hpp"
#include <iostream>
#include <iomanip>

int main() {
    StrategyGenerator gen(2, 2, 2);

    for (StrategyType type : {StrategyType::IJK, StrategyType::IKJ, StrategyType::TILED_4x4}) {
        std::cout << "\nStrategy: " << gen.strategyName(type) << "\n";

        auto irList = gen.generate(type);
        for (const auto& instr : irList) {
            std::cout << instr.toString() << " | 0x"
                      << std::hex << std::setw(6) << std::setfill('0') 
                      << instr.encodeToISA() << std::dec << "\n";
        }
    }

    return 0;
}
