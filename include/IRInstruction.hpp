#ifndef IR_INSTRUCTION_HPP
#define IR_INSTRUCTION_HPP

#include <string>

enum class OpType { PROG, EXE };

class IRInstruction {
public:
    OpType op;
    int ptr;
    bool read;
    bool write;
    int row;

    IRInstruction(OpType op, int ptr, bool read, bool write, int row);


    std::string toString() const;
    unsigned int encodeToISA() const;
    
    int getRow() const;  // ✅ NEW
};

#endif
