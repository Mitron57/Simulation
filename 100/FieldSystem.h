#ifndef SIMULATION_FIELDSYSTEM_H
#define SIMULATION_FIELDSYSTEM_H

#include <fstream>
#include <random>

using namespace ECS;

namespace Solution {
    struct FieldSystem final : System {
        void onAwake() override {
            const std::shared_ptr fieldEntity {Filter<Field>::filter()[0]};
            const auto [fieldComponent] {
                Manager::getComponents<Field>(fieldEntity)
            };
            std::random_device randomDevice {};
            std::mt19937 engine {randomDevice()};
            std::uniform_int_distribution<std::uint8_t> letter(
                'a', lettersCount - 1 + 'a'
            );
            const std::uint8_t bacteria {letter(engine)};
            std::ofstream workDat {"work.dat"};
            if (workDat.is_open()) {
                workDat << "Initial array:" << std::endl;
                workDat << "Bacteria hides under letter " << bacteria
                        << std::endl;
                for (std::uint32_t i {}; i < fieldSize; ++i) {
                    fieldComponent->current.emplace_back();
                    for (std::uint32_t j {}; j < fieldSize; ++j) {
                        const std::uint8_t sign {letter(engine)};
                        workDat << sign << " ";
                        if (sign == bacteria) {
                            fieldComponent->current[i].emplace_back(1);
                            const auto entity {Manager::createEntity()};
                            Manager::addComponent<Health>(entity);
                            Manager::addComponent<Position>(entity);
                            const auto [position] {
                                Manager::getComponents<Position>(entity)
                            };
                            position->posY = i;
                            position->posX = j;
                        } else {
                            fieldComponent->current[i].emplace_back(0);
                        }
                    }
                    workDat << std::endl;
                }
            }
            fieldComponent->future = fieldComponent->current;
        }
    };
}  // namespace Solution
#endif  // SIMULATION_FIELDSYSTEM_H
