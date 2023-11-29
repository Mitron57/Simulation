#ifndef SIMULATION_LIFESYSTEM_H
#define SIMULATION_LIFESYSTEM_H
#include <utility>

using namespace Engine;

namespace Solution {
    struct LifeSystem final : System {
        bool onUpdate() override {
            const std::shared_ptr fieldEntity {Filter<Field>::filter()[0]};
            const auto [field] {World::getComponents<Field>(*fieldEntity)};
            std::vector entities {Filter<Health, With<Position>>::filter()};
            for (std::int32_t i {}; i < fieldSize; ++i) {
                for (std::int32_t j {}; j < fieldSize; ++j) {
                    auto entity {std::ranges::find_if(
                                entities,
                                [i, j](std::shared_ptr<Entity>& entity) {
                                    auto [position] {
                                        World::getComponents<Position>(*entity)
                                    };
                                    return position->posX == j &&
                                           position->posY == i;
                                }
                            )};
                    if (const auto count {countNear(field->current, j, i)};
                        count == 2 || count == 3) {
                        if (field->current[i][j] == 0) {
                            const auto bacteria = World::createEntity();
                            World::addComponent<Health>(*bacteria);
                            World::addComponent<Position>(*bacteria);
                            const auto [position] {
                                World::getComponents<Position>(*bacteria)
                            };
                            position->posX = j;
                            position->posY = i;
                            field->future[i][j] = 1;
                        } else {
                            auto [health] {World::getComponents<Health>(**entity)};
                            if (++health->age == 12) {
                                field->future[i][j] = 0;
                                World::deleteEntity(**entity);
                                entities.erase(entity);
                            } else {
                                field->future[i][j] = health->age;
                            }
                        }
                    } else if (entity != entities.end()) {
                        field->future[i][j] = 0;
                        World::deleteEntity(**entity);
                        entities.erase(entity);
                    }
                }
            }
            field->current = std::move(field->future);
            field->future.clear();
            for (std::uint32_t i {}; i < fieldSize; ++i) {
                field->future.emplace_back();
                for (std::uint32_t j {}; j< fieldSize; ++j) {
                    field->future[i].emplace_back();
                }
            }
            return ++currentGeneration <= maxGeneration;
        }
    };
}  // namespace Solution

#endif  // SIMULATION_LIFESYSTEM_H
