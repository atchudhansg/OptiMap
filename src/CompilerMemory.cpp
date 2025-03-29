#include "CompilerMemory.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;

CompilerMemory::CompilerMemory(const std::string& filename) : filename(filename) {}

void CompilerMemory::load() {
    std::ifstream inputFile(filename);
    if (inputFile) {
        inputFile >> jsonData;
        for (auto& [key, value] : jsonData.items()) {
            std::string strategy = value["strategy"];
            int cost = value["cost"];
            memory[key] = {strategy, cost};
        }
    }
}

void CompilerMemory::save() {
    for (const auto& [key, entry] : memory) {
        jsonData[key] = {{"strategy", entry.strategy}, {"cost", entry.cost}};
    }

    std::ofstream outputFile(filename);
    outputFile << jsonData.dump(4);
}

void CompilerMemory::log(const std::string& matrix_size, const std::string& strategy, int cost) {
    memory[matrix_size] = {strategy, cost};
    save();
}

std::string CompilerMemory::getBestStrategy(const std::string& matrix_size) {
    if (memory.find(matrix_size) != memory.end()) {
        return memory[matrix_size].strategy;
    }
    return "";
}
