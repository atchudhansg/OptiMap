#ifndef IRINSTRUCTION_HPP
#define IRINSTRUCTION_HPP

#include <string>
#include <cstdint>

enum class IROpcode {
    NOOP,
    PROG,
    EXE,
    END
};

class IRInstruction {
public:
    // Constructor
    IRInstruction(IROpcode op, uint8_t pointer = 0, bool read = false, bool write = false, uint16_t row = 0);

    // Methods
    std::string toString() const;
    uint32_t encodeToISA() const;

private:
    IROpcode opcode;
    uint8_t pointer;     // Core/Read Pointer (6 bits)
    bool read;           // 1 bit
    bool write;          // 1 bit
    uint16_t row_addr;   // 9 bits
};

#endif // IRINSTRUCTION_HPP
