#include "MemoryMapper.hpp"
#include <sstream>

MemoryMapper::MemoryMapper(int bank_count, int row_size)
    : banks(bank_count), rowCapacity(row_size) {}

std::tuple<int, int, int> MemoryMapper::map(int matrixId, int row, int col) {
    int flatIndex = row * rowCapacity + col;
    int bank = matrixId % banks;
    int memRow = flatIndex / rowCapacity;
    int offset = flatIndex % rowCapacity;
    return {bank, memRow, offset};
}

std::string MemoryMapper::toString(int matrixId, int row, int col) {
    auto [bank, memRow, offset] = map(matrixId, row, col);
    std::ostringstream oss;
    oss << "Matrix[" << matrixId << "](" << row << "," << col << ") → "
        << "Bank: " << bank << ", Row: " << memRow << ", Offset: " << offset;
    return oss.str();
}
