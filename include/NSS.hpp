#ifndef NSS_HPP
#define NSS_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class NSS {
public:
    // Construct from JSON directly (instead of from file path)
    NSS(nlohmann::json& memory);

    // Predict best permutation from candidates
    std::string evolveAndSelect(const std::vector<std::string>& candidates, const std::string& fingerprint);

    // Update cost/fingerprint memory
    void updateFitness(const std::string& fingerprint, const std::string& chosenPerm, int cost);

    // Export updated memory for saving
    nlohmann::json exportJSON() const;

private:
    nlohmann::json memory;
};

#endif // NSS_HPP
