#include "HHESS.hpp"
#include <fstream>
#include "nlohmann/json/include/nlohmann/json.hpp"
#include <random>
#include <algorithm>

HHESS::HHESS(const std::string& dbFile) : dbFile(dbFile) {
    load();
}

void HHESS::load() {
    std::ifstream in(dbFile);
    if (in) in >> memory;
    else memory = nlohmann::json::object();
}

void HHESS::save() {
    std::ofstream out(dbFile);
    out << memory.dump(4);
}

void HHESS::recordResult(const std::string& matrixSize, const std::string& strategy, int cost) {
    auto& record = memory[matrixSize];
    if (!record.contains("strategy") || cost < record["cost"]) {
        record["strategy"] = strategy;
        record["cost"] = cost;
        save();
    }
}

std::string HHESS::selectBestStrategy(const std::string& matrixSize) {
    if (memory.contains(matrixSize)) return memory[matrixSize]["strategy"];
    return mutateStrategy("");  // No history, mutate from scratch
}

std::string HHESS::mutateStrategy(const std::string& current) {
    static std::vector<std::string> strategies = {"ijk", "ikj", "tiled_4x4"};
    static std::random_device rd;
    static std::mt19937 gen(rd());

    // Choose randomly for now, later replace with intelligent mutation
    std::uniform_int_distribution<> dis(0, strategies.size() - 1);
    return strategies[dis(gen)];
}
