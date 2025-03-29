#include "StrategyGenerator.hpp"
#include <iostream>

StrategyGenerator::StrategyGenerator(int rowsA, int colsA, int colsB)
    : M(rowsA), N(colsA), K(colsB) {}

std::vector<IRInstruction> StrategyGenerator::generate(StrategyType type) const {
    std::vector<IRInstruction> irList;

    switch (type) {
        case StrategyType::IJK:
            for (int i = 0; i < M; ++i)
                for (int j = 0; j < K; ++j)
                    for (int k = 0; k < N; ++k) {
                        // Simplified dummy IR (actual memory mapping later)
                        irList.emplace_back(IROpcode::PROG, 1);
                        irList.emplace_back(IROpcode::EXE, 1, true, false, i*10 + j); // fake row address
                    }
            break;

        case StrategyType::IKJ:
            for (int i = 0; i < M; ++i)
                for (int k = 0; k < N; ++k)
                    for (int j = 0; j < K; ++j) {
                        irList.emplace_back(IROpcode::PROG, 1);
                        irList.emplace_back(IROpcode::EXE, 1, true, false, i*10 + j);
                    }
            break;

        case StrategyType::TILED_4x4:
            for (int i = 0; i < M; i += 4)
                for (int j = 0; j < K; j += 4)
                    for (int k = 0; k < N; k += 4) {
                        irList.emplace_back(IROpcode::PROG, 2);
                        irList.emplace_back(IROpcode::EXE, 2, true, false, i + j + k); // dummy
                    }
            break;
    }

    return irList;
}

std::string StrategyGenerator::strategyName(StrategyType type) const {
    switch (type) {
        case StrategyType::IJK: return "ijk";
        case StrategyType::IKJ: return "ikj";
        case StrategyType::TILED_4x4: return "tiled_4x4";
        default: return "unknown";
    }
}
