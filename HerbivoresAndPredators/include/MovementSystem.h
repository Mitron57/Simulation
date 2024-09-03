#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

using namespace ECS;

namespace Solution {
    struct MovementSystem final : System {
        void onAwake() override {}
        bool onUpdate() override;
    private:
        template <typename T>
        static void moveEntity(const std::shared_ptr<Position>& position);
    };
}  // namespace Solution
#endif
