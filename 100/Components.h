#ifndef SIMULATION_COMPONENTS_H
#define SIMULATION_COMPONENTS_H

using namespace ECS;

namespace Solution {
    static std::uint32_t maxGeneration {}, fieldSize {};
    static std::uint8_t lettersCount {};
    static std::uint32_t currentGeneration {1};

    struct Health final : Component {
        std::uint32_t age = 1;
    };

    struct Field final : Component {
        std::vector<std::vector<std::uint32_t>> current {}, future {};
    };

    struct Position final : Component {
        std::uint32_t posX {};
        std::uint32_t posY {};
    };
}
#endif  // SIMULATION_COMPONENTS_H
