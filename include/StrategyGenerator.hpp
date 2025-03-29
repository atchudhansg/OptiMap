#ifndef STRATEGY_GENERATOR_HPP
#define STRATEGY_GENERATOR_HPP

#include "IRInstruction.hpp"
#include <vector>
#include <string>

enum class StrategyType {
    IJK,
    IKJ,
    TILED_4x4
};

class StrategyGenerator {
public:
    StrategyGenerator(int rowsA, int colsA, int colsB);

    std::vector<IRInstruction> generate(StrategyType type) const;
    std::string strategyName(StrategyType type) const;

private:
    int M, N, K; // Matrix dimensions
};

#endif // STRATEGY_GENERATOR_HPP
