#ifndef LLVM_FRONTEND_HPP
#define LLVM_FRONTEND_HPP

#include <string>

class LLVMFrontend {
public:
    // Parses an LLVM IR (.ll) file and prints meaningful info
    static void parseIRFile(const std::string& filename);
};

#endif // LLVM_FRONTEND_HPP
