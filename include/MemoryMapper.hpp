#ifndef MEMORY_MAPPER_HPP
#define MEMORY_MAPPER_HPP

#include <tuple>
#include <string>

class MemoryMapper {
public:
    MemoryMapper(int bank_count = 4, int row_size = 256);
    
    std::tuple<int, int, int> map(int matrixId, int row, int col);
    std::string toString(int matrixId, int row, int col);

private:
    int banks;
    int rowCapacity;
};

#endif // MEMORY_MAPPER_HPP
