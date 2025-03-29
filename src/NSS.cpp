#include "NSS.hpp"
#include <limits>
#include <random>

NSS::NSS(nlohmann::json& memoryRef) {
    memory = memoryRef;
}

std::string NSS::evolveAndSelect(const std::vector<std::string>& candidates, const std::string& fingerprint) {
    if (memory.contains(fingerprint)) {
        const auto& options = memory[fingerprint];
        double bestCost = std::numeric_limits<double>::max();
        std::string bestPerm;

        for (const auto& perm : candidates) {
            if (options.contains(perm)) {
                double cost = options[perm];
                if (cost < bestCost) {
                    bestCost = cost;
                    bestPerm = perm;
                }
            }
        }

        if (!bestPerm.empty()) return bestPerm;
    }

    // 🔀 Random fallback (no known best yet)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, candidates.size() - 1);
    return candidates[dis(gen)];
}

void NSS::updateFitness(const std::string& fingerprint, const std::string& chosenPerm, int cost) {
    memory[fingerprint][chosenPerm] = cost;
}

nlohmann::json NSS::exportJSON() const {
    return memory;
}
