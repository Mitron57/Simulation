#ifndef COMPONENTS_H
#define COMPONENTS_H

using namespace ECS;

namespace Solution {
    struct Herbivore final : Component {
        static constexpr char sign {'O'};
    };

    struct Predator final : Component {
        static constexpr char sign {'X'};
    };

    struct Health final : Component {
        std::uint32_t age {}, satiety {100}, birthday {1};
    };

    struct Position final : Component {
        std::int32_t x {}, y {};
    };
}
#endif //COMPONENTS_H
