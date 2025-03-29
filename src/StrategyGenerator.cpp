#include "StrategyGenerator.hpp"
#include "IRInstruction.hpp"
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
                        irList.emplace_back(OpType::PROG, 1, false, false, 0); // dummy
                        irList.emplace_back(OpType::EXE, 1, true, false, i * 10 + j); // simulated row
                    }
            break;

        case StrategyType::IKJ:
            for (int i = 0; i < M; ++i)
                for (int k = 0; k < N; ++k)
                    for (int j = 0; j < K; ++j) {
                        irList.emplace_back(OpType::PROG, 1, false, false, 0);
                        irList.emplace_back(OpType::EXE, 1, true, false, i * 10 + j);
                    }
            break;

        case StrategyType::TILED_4x4:
            for (int i = 0; i < M; i += 4)
                for (int j = 0; j < K; j += 4)
                    for (int k = 0; k < N; k += 4) {
                        irList.emplace_back(OpType::PROG, 2, false, false, 0);
                        irList.emplace_back(OpType::EXE, 2, true, false, i + j + k);
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
