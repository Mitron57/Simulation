#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

using namespace ECS;

namespace Solution {
    struct MovementSystem final : System {
        void onAwake() override {}
        bool onUpdate() override;
    };
}  // namespace Solution
#endif
