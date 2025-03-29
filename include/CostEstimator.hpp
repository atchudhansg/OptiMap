#ifndef COST_ESTIMATOR_HPP
#define COST_ESTIMATOR_HPP

#include "IRInstruction.hpp"
#include <vector>

class CostEstimator {
public:
    int estimateCost(const std::vector<IRInstruction>& irList) const;
};

#endif // COST_ESTIMATOR_HPP
