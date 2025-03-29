#include "CostEstimator.hpp"
#include <unordered_set>

int CostEstimator::estimateCost(const std::vector<IRInstruction>& irList) const {
    int cost = 0;
    int prog_count = 0;
    int exe_count = 0;
    int read_ops = 0;
    int write_ops = 0;

    int last_row = -1;
    int row_switch_penalty = 2; // Penalty for switching row addresses

    for (const auto& instr : irList) {
        cost += 1; // Base cost per instruction

        if (instr.toString().find("PROG") != std::string::npos) prog_count++;
        if (instr.toString().find("EXE") != std::string::npos) exe_count++;
        if (instr.toString().find("R: 1") != std::string::npos) read_ops++;
        if (instr.toString().find("W: 1") != std::string::npos) write_ops++;

        // Add penalty if row changes
        if (((instr.encodeToISA() & 0x1FF) != last_row) && last_row != -1){
            cost += row_switch_penalty;
        }
        last_row = instr.encodeToISA() & 0x1FF;
    }

    // Optional: weight different types
    cost += prog_count * 2;
    cost += exe_count * 1;
    cost += write_ops * 3;

    return cost;
}
