#include "StrategyGenerator.hpp"
#include <iostream>
#include <sstream>

StrategyGenerator::StrategyGenerator(const std::string& strategyName, const std::vector<int>& dims)
    : name(strategyName), shape(dims) {}

std::string StrategyGenerator::getName() const {
    return name;
}

std::vector<IRInstruction> StrategyGenerator::generate() const {
    std::vector<IRInstruction> irList;

    // Example logic: Each element in shape represents a dimension
    // We'll create a nested loop for each dimension
    // For now: generate 2 ops (PROG, EXE) per inner-most loop iteration

    std::function<void(std::vector<int>&, int)> recursiveEmit =
        [&](std::vector<int>& indices, int dim) {
            if (dim == shape.size()) {
                // Simulated offset = sum of indices (for uniqueness)
                int offset = 0;
                for (int v : indices) offset += v;

                irList.emplace_back(OpType::PROG, shape.size(), false, false, 0);
                irList.emplace_back(OpType::EXE, shape.size(), true, false, offset);
                return;
            }

            for (int i = 0; i < shape[dim]; ++i) {
                indices[dim] = i;
                recursiveEmit(indices, dim + 1);
            }
        };

    std::vector<int> indexTrack(shape.size(), 0);
    recursiveEmit(indexTrack, 0);

    return irList;
}
