#ifndef STRATEGY_GENERATOR_HPP
#define STRATEGY_GENERATOR_HPP

#include "IRInstruction.hpp"
#include <vector>
#include <string>

class StrategyGenerator {
public:
    StrategyGenerator(const std::string& strategyName, const std::vector<int>& dims);

    std::vector<IRInstruction> generate() const;
    std::string getName() const;

private:
    std::string name;
    std::vector<int> shape;  // e.g., {6, 6, 6}
};

#endif // STRATEGY_GENERATOR_HPP
