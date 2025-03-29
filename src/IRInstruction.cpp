#include "IRInstruction.hpp"
#include <sstream>
#include <iomanip>

IRInstruction::IRInstruction(OpType op, int ptr, bool read, bool write, int row)
    : op(op), ptr(ptr), read(read), write(write), row(row) {}

std::string IRInstruction::toString() const {
    std::ostringstream oss;
    oss << "[IR] Op: " << (op == OpType::PROG ? "PROG" : "EXE")
        << ", Ptr: " << ptr
        << ", R: " << read
        << ", W: " << write
        << ", Row: " << row;
    return oss.str();
}

unsigned int IRInstruction::encodeToISA() const {
    unsigned int isa = 0;
    isa |= (op == OpType::EXE) ? (1 << 23) : 0;     // Bit 23: EXE or PROG
    isa |= (ptr & 0xF) << 19;                       // Bits 22–19: pointer
    isa |= (read ? 1 : 0) << 18;                    // Bit 18: read
    isa |= (write ? 1 : 0) << 17;                   // Bit 17: write
    isa |= (row & 0x1FF);                           // Bits 8–0: row
    return isa;
}

// ✅ NEW
int IRInstruction::getRow() const {
    return encodeToISA() & 0x1FF; // Lower 9 bits
}
