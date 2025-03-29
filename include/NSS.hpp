#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "MLPModel.hpp"

class NSS {
public:
    // Construct from JSON + specify how many input features for the MLP
    NSS(nlohmann::json& memoryRef, size_t featureCount);

    // Decide best strategy among candidates
    std::string evolveAndSelect(const std::vector<std::string>& candidates,
                                const std::string& fingerprint,
                                const std::vector<int>& dims);

    // Update memory & train MLP with actual cost
    void updateFitness(const std::string& fingerprint,
                       const std::string& chosenStrategy,
                       const std::vector<int>& dims,
                       int cost);

    // Return the updated memory for saving
    nlohmann::json exportJSON() const;

private:
    nlohmann::json memory;
    MLPModel model;  // the advanced MLP

    // Convert dimensions + strategy name => feature vector
    std::vector<int> makeFeatures(const std::vector<int>& dims, const std::string& strategy) const;
};
