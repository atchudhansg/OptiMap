#pragma once
#include <string>
#include <vector>
#include "IRInstruction.hpp"
#include "nlohmann/json/include/nlohmann/json.hpp"

class HHESS {
public:
    HHESS(const std::string& dbFile);
    std::string selectBestStrategy(const std::string& matrixSize);
    void recordResult(const std::string& matrixSize, const std::string& strategy, int cost);

private:
    std::string dbFile;
    nlohmann::json memory;

    void load();
    void save();
    std::string mutateStrategy(const std::string& current);
};
