#include "IRInstruction.hpp"
#include <iostream>
#include <iomanip>

int main() {
    IRInstruction instr(IROpcode::EXE, 5, true, false, 42);

    std::cout << instr.toString() << std::endl;
    std::cout << "Encoded ISA: 0x" << std::hex << std::setw(6) << std::setfill('0') 
              << instr.encodeToISA() << std::endl;

    return 0;
}
