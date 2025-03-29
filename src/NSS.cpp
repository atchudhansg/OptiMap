#include "NSS.hpp"
#include <limits>
#include <sstream>

NSS::NSS(nlohmann::json& memoryRef, size_t featureCount)
    : memory(memoryRef),
      model(featureCount)  // Initialize MLP with required # of input features
{
}

std::string NSS::evolveAndSelect(const std::vector<std::string>& candidates,
                                 const std::string& fingerprint,
                                 const std::vector<int>& dims)
{
    // 1) If we have data for this fingerprint, pick best known
    if (memory.contains(fingerprint)) {
        const auto& knownStrategies = memory[fingerprint];
        double bestKnownCost = std::numeric_limits<double>::infinity();
        std::string bestKnownStrategy;

        for (const auto& candidate : candidates) {
            if (knownStrategies.contains(candidate)) {
                double c = knownStrategies[candidate]; // actual known cost
                if (c < bestKnownCost) {
                    bestKnownCost = c;
                    bestKnownStrategy = candidate;
                }
            }
        }
        if (!bestKnownStrategy.empty()) {
            return bestKnownStrategy;
        }
    }

    // 2) Otherwise, predict cost for each candidate using MLP
    double bestPredictedCost = std::numeric_limits<double>::infinity();
    std::string bestPredictedStrategy;

    for (const auto& candidate : candidates) {
        auto features = makeFeatures(dims, candidate);
        double pred = model.predict(features);
        if (pred < bestPredictedCost) {
            bestPredictedCost = pred;
            bestPredictedStrategy = candidate;
        }
    }

    return bestPredictedStrategy;
}

void NSS::updateFitness(const std::string& fingerprint,
                        const std::string& chosenStrategy,
                        const std::vector<int>& dims,
                        int cost)
{
    // Store in JSON memory
    memory[fingerprint][chosenStrategy] = cost;

    // Convert to features & train MLP with actual cost
    auto features = makeFeatures(dims, chosenStrategy);
    model.update(features, static_cast<double>(cost));
}

nlohmann::json NSS::exportJSON() const {
    return memory;
}

std::vector<int> NSS::makeFeatures(const std::vector<int>& dims, const std::string& strategy) const {
    // Basic example: combine dims + numeric strategy ID
    std::vector<int> feats;
    feats.reserve(dims.size() + 1);

    for (auto d : dims) {
        feats.push_back(d);
    }
    // Convert strategy string to an integer
    int sumChar = 0;
    for (char c : strategy) {
        sumChar += static_cast<unsigned char>(c);
    }
    feats.push_back(sumChar);

    return feats;
}
