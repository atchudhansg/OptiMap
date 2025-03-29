#include "LLVMFrontend.hpp"
#include <fstream>
#include <iostream>
#include <regex>

void LLVMFrontend::parseIRFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "❌ Could not open LLVM IR file: " << filename << std::endl;
        return;
    }

    std::cout << "📄 LLVM IR File: " << filename << "\n";

    std::string line;
    std::regex loadRegex(R"(\s*%(\w+)\s*=\s*load\s+i32)");
    std::regex mulRegex(R"(\s*%(\w+)\s*=\s*mul\s+i32\s+%(\w+),\s+%(\w+))");
    std::regex storeRegex(R"(\s*store\s+i32\s+%(\w+),\s+i32\*\s+%(\w+))");

    while (std::getline(file, line)) {
        if (std::smatch match; std::regex_search(line, match, mulRegex)) {
            std::cout << "🔢 MUL Detected: " << match[1] << " = " << match[2] << " * " << match[3] << "\n";
        } else if (std::regex_search(line, match, loadRegex)) {
            std::cout << "📥 LOAD Detected: " << match[1] << "\n";
        } else if (std::regex_search(line, match, storeRegex)) {
            std::cout << "📤 STORE Detected: " << match[1] << " → " << match[2] << "\n";
        }
    }

    file.close();
}
