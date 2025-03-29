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

// ==================================================
// Step 9: Main entry point using advanced MLP-based NSS
// ==================================================
int main() {
    std::cout << "\n========== LLVM IR Input Analysis ==========\n";
    LLVMFrontend::parseIRFile("../matrix_mul.ll");

    // 1) Define chain dimensions for a single chain:
    //    e.g., (10x5)*(5x2)*(2x3)
    std::vector<int> dimensions = {10, 5, 2, 3};  
    // Build a "fingerprint" string like "10x5x2x3"
    std::string fingerprint;
    for (size_t i = 0; i < dimensions.size(); ++i) {
        fingerprint += std::to_string(dimensions[i]);
        if (i != dimensions.size() - 1) fingerprint += "x";
    }

    // 2) In a more sophisticated setup, you'd have multiple chain permutations
    //    or multiple strategies. For now, we just have a single "chain" as a fallback.
    //    If you want to expand to multiple permutations or different loop orders,
    //    define them here. Example:
    std::vector<std::string> validChains = {fingerprint};

    // 3) Load existing memory from JSON, or initialize if empty
    std::string memoryFile = "nss_memory.json";
    nlohmann::json memoryJson;
    {
        std::ifstream inFile(memoryFile);
        if (inFile.peek() != std::ifstream::traits_type::eof()) {
            inFile >> memoryJson;
        } else {
            std::cout << "⚠️  NSS memory file is empty. Initializing new JSON.\n";
            memoryJson = nlohmann::json::object();
        }
    }

    // 4) Create the advanced NSS object with MLP
    //    The MLP expects #features = (number_of_dimensions + 1) 
    //    because we typically add a "strategy ID" as an extra feature.
    //    Since we currently have only 1 chain, this is minimal. 
    //    But let's keep it consistent with the advanced approach.
    size_t featureCount = dimensions.size() + 1; 
    NSS neuroSelector(memoryJson, featureCount);

    // 5) Let NSS pick (or predict) the best chain from validChains
    //    Note that in the advanced pipeline, the signature is:
    //      evolveAndSelect(const std::vector<std::string>&, 
    //                      const std::string& fingerprint,
    //                      const std::vector<int>& dims)
    std::string chosenChain = neuroSelector.evolveAndSelect(validChains, fingerprint, dimensions);
    std::cout << "\n✅ Selected Matrix Chain: " << chosenChain << "\n";

    // 6) Convert the chosen chain back into dimension vector
    std::vector<int> shape;
    {
        std::stringstream ss(chosenChain);
        std::string token;
        while (std::getline(ss, token, 'x')) {
            shape.push_back(std::stoi(token));
        }
    }

    // 7) IR Generation for the chosen matrix chain
    StrategyGenerator gen(chosenChain, shape);
    auto irList = gen.generate();  // The generator returns a list of IR instructions

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

    // 8) Cost Estimation (this is the actual cost of the chosen strategy)
    CostEstimator estimator;
    int cost = estimator.estimateCost(irList);
    std::cout << "Estimated Cost: " << cost << "\n";

    // 9) Update NSS memory & the MLP model with real cost
    //    The advanced method signature is:
    //      updateFitness(const std::string&, const std::string&, 
    //                    const std::vector<int>&, int)
    neuroSelector.updateFitness(fingerprint, chosenChain, shape, cost);

    // 10) Save updated memory to disk
    {
        std::ofstream outFile(memoryFile);
        outFile << neuroSelector.exportJSON().dump(4);
    }

    return 0;
}
