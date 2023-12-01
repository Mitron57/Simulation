#ifndef SIMULATION_LIFESYSTEM_H
#define SIMULATION_LIFESYSTEM_H

using namespace ECS;

namespace Solution {
    struct LifeSystem final : System {
        bool onUpdate() override {
            const std::shared_ptr fieldEntity {Filter<Field>::filter()[0]};
            const auto [field] {Manager::getComponents<Field>(fieldEntity)};
            std::vector entities {Filter<Health, With<Position>>::filter()};
            std::vector<std::shared_ptr<Entity>> entityToDelete {};
            for (std::int32_t i {}; i < fieldSize; ++i) {
                for (std::int32_t j {}; j < fieldSize; ++j) {
                    auto entity {std::ranges::find_if(
                        entities,
                        [i, j](std::shared_ptr<Entity>& entity) {
                            if (entity) {
                                auto [position] {
                                    Manager::getComponents<Position>(entity)
                                };
                                return position->posX == j &&
                                       position->posY == i;
                            }
                            return false;
                        }
                    )};
                    const auto count {countNear(field->current, j, i)};
                    if (count == 2 || count == 3) {
                        if (field->current[i][j] == 0 && count == 3) {
                            const auto bacteria = Manager::createEntity();
                            Manager::addComponent<Health>(bacteria);
                            Manager::addComponent<Position>(bacteria);
                            const auto [position] {
                                Manager::getComponents<Position>(bacteria)
                            };
                            position->posX = j;
                            position->posY = i;
                            field->future[i][j] = 1;
                        } else if (field->current[i][j] != 0) {
                            auto [health] {
                                Manager::getComponents<Health>(*entity)
                            };
                            if (++health->age == 13) {
                                field->future[i][j] = 0;
                                entityToDelete.push_back(*entity);
                            } else {
                                field->future[i][j] = health->age;
                            }
                        }
                    } else if (entity != entities.end()) {
                        field->future[i][j] = 0;
                        entityToDelete.push_back(*entity);
                    }
                }
            }
            for (auto& entity : entityToDelete) {
                Manager::deleteEntity(std::move(entity));
            }
            field->current = std::move(field->future);
            for (std::uint32_t i {}; i < fieldSize; ++i) {
                field->future.emplace_back();
                for (std::uint32_t j {}; j < fieldSize; ++j) {
                    field->future[i].emplace_back();
                }
            }
            return ++currentGeneration <= maxGeneration;
        }
    };
}  // namespace Solution

#endif  // SIMULATION_LIFESYSTEM_H
