#ifndef SIMULATION_UTILS_H
#define SIMULATION_UTILS_H

#include <iostream>

using namespace ECS;

namespace Solution {
    template <typename F>
    std::size_t countNear(const F& field, std::int32_t x, std::int32_t y) {
        std::size_t count {};
        for (std::int32_t i = y - 1; i < y + 2; ++i) {
            for (std::int32_t j = x - 1; j < x + 2; ++j) {
                if (0 <= i && i < fieldSize && 0 <= j && j < fieldSize &&
                    field[i][j] != 0) {
                    ++count;
                }
            }
        }
        return field[y][x] != 0 ? count - 1 : count;
    }
}  // namespace Solution
#endif  // SIMULATION_UTILS_H
