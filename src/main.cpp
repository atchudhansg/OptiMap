#include "IRInstruction.hpp"
#include "StrategyGenerator.hpp"
#include "CostEstimator.hpp"
#include "CompilerMemory.hpp"
#include "MemoryMapper.hpp"
#include "LLVMFrontend.hpp"
#include "NSS.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>

// ✅ Use actual matrix dimensions for n matrix chain multiplication
int main() {
    std::cout << "\n========== LLVM IR Input Analysis ==========\n";
    LLVMFrontend::parseIRFile("../matrix_mul.ll");

    // 🔢 Define chain dimensions (M0 x M1) * (M1 x M2) * ... * (Mn-1 x Mn)
    std::vector<int> dimensions = {10, 5, 2, 3};  // Example: (10x5)*(5x2)*(2x3)
    std::string fingerprint;
    for (size_t i = 0; i < dimensions.size(); ++i) {
        fingerprint += std::to_string(dimensions[i]);
        if (i != dimensions.size() - 1) fingerprint += "x";
    }

    // Only 1 valid order — original chain
    std::vector<std::string> validChains = {fingerprint};

    // 🛡️ Load NSS memory safely
    std::string memoryFile = "nss_memory.json";
    nlohmann::json memoryJson;
    std::ifstream inFile(memoryFile);
    if (inFile.peek() != std::ifstream::traits_type::eof()) {
        inFile >> memoryJson;
    } else {
        std::cout << "⚠️  NSS memory file is empty. Initializing new JSON.\n";
        memoryJson = nlohmann::json::object();
    }
    inFile.close();

    // 🧠 Evolve/select based on NSS memory
    NSS neuroSelector(memoryJson);
    std::string chosenChain = neuroSelector.evolveAndSelect(validChains, fingerprint);
    std::cout << "\n✅ Selected Matrix Chain: " << chosenChain << "\n";

    // 🔁 Convert string back to vector<int>
    std::vector<int> shape;
    std::stringstream ss(chosenChain);
    std::string token;
    while (std::getline(ss, token, 'x')) {
        shape.push_back(std::stoi(token));
    }

    // ⚙️ IR Generation for given matrix chain
    StrategyGenerator gen(chosenChain, shape);
    auto irList = gen.generate();

    std::cout << "\n📦 Generated IR:\n";
    MemoryMapper memoryMapper;
    for (const auto& instr : irList) {
        std::cout << instr.toString() << " | 0x"
                  << std::hex << std::setw(6) << std::setfill('0')
                  << instr.encodeToISA() << std::dec << "\n";

        int actualRow = instr.getRow();
        auto mapping = memoryMapper.toString(0, actualRow, 0);
        std::cout << "    ↳ " << mapping << "\n";
    }

    // 💰 Cost Estimation
    CostEstimator estimator;
    int cost = estimator.estimateCost(irList);
    std::cout << "Estimated Cost: " << cost << "\n";

    // 💾 Update NSS memory and write to disk
    neuroSelector.updateFitness(fingerprint, chosenChain, cost);

    std::ofstream outFile(memoryFile);
    outFile << neuroSelector.exportJSON().dump(4);
    outFile.close();

    return 0;
}
