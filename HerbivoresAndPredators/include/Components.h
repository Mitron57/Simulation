#ifndef COMPONENTS_H
#define COMPONENTS_H

using namespace ECS;

namespace Solution {
    struct Herbivore final : Component {
        static constexpr char sign {'O'};
        static std::uint64_t dead;
    };
    std::uint64_t Herbivore::dead = 0;

    struct Predator final : Component {
        static constexpr char sign {'X'};
        static std::uint64_t dead;
    };
    std::uint64_t Predator::dead = 0;

    struct Grass final : Component {
        static constexpr char sign {'#'};
    };

    struct Health final : Component {
        std::uint32_t age {}, satiety {100}, birthday {1};
    };

    struct Position final : Component {
        std::int32_t x {}, y {};
    };
}
#endif //COMPONENTS_H
