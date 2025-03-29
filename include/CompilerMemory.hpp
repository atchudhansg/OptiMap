#ifndef COMPILER_MEMORY_HPP
#define COMPILER_MEMORY_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>  // Now this will resolve correctly

struct CompilerMemoryEntry {
    std::string strategy;
    int cost;
};

class CompilerMemory {
public:
    CompilerMemory(const std::string& filename);
    void load();
    void save();
    void log(const std::string& matrix_size, const std::string& strategy, int cost);
    std::string getBestStrategy(const std::string& matrix_size);

private:
    std::string filename;
    std::unordered_map<std::string, CompilerMemoryEntry> memory;
    nlohmann::json jsonData;
};

#endif // COMPILER_MEMORY_HPP
