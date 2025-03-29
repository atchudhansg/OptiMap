#include "IRInstruction.hpp"
#include <sstream>
#include <iomanip>

IRInstruction::IRInstruction(IROpcode op, uint8_t ptr, bool rd, bool wr, uint16_t row)
    : opcode(op), pointer(ptr), read(rd), write(wr), row_addr(row) {}

std::string IRInstruction::toString() const {
    std::ostringstream oss;
    oss << "[IR] Op: ";

    switch (opcode) {
        case IROpcode::NOOP: oss << "NOOP"; break;
        case IROpcode::PROG: oss << "PROG"; break;
        case IROpcode::EXE:  oss << "EXE";  break;
        case IROpcode::END:  oss << "END";  break;
    }

    oss << ", Ptr: " << +pointer
        << ", R: " << read
        << ", W: " << write
        << ", Row: " << row_addr;

    return oss.str();
}

uint32_t IRInstruction::encodeToISA() const {
    uint32_t word = 0;

    uint8_t op_bits;
    switch (opcode) {
        case IROpcode::NOOP: op_bits = 0b00; break;
        case IROpcode::PROG: op_bits = 0b01; break;
        case IROpcode::EXE:  op_bits = 0b10; break;
        case IROpcode::END:  op_bits = 0b11; break;
    }

    word |= (op_bits << 22);          // Opcode (2 bits)
    word |= (pointer & 0x3F) << 16;   // Pointer (6 bits)
    word |= (read ? 1 : 0) << 15;     // Read bit
    word |= (write ? 1 : 0) << 14;    // Write bit
    word |= (row_addr & 0x1FF);       // 9-bit row address

    return word;
}
