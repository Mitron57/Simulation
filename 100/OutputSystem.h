#ifndef SIMULATION_OUTPUTSYSTEM_H
#define SIMULATION_OUTPUTSYSTEM_H

#include <fstream>

namespace Solution {
    struct OutputSystem final : System {
        std::ofstream file {"work.out"};

        ~OutputSystem() override {
            file.close();
        }

        bool onUpdate() override {
            const std::shared_ptr fieldEntity {Filter<Field>::filter()[0]};
            const auto [fieldComponent] {
                Manager::getComponents<Field>(fieldEntity)
            };
            file << "Current generation: " << currentGeneration << std::endl;
            file << "Field: " << std::endl;
            std::cout << "Current generation: " << currentGeneration << std::endl;
            std::cout << "Field: " << std::endl;
            for (const auto& line : fieldComponent->current) {
                for (const auto age : line) {
                    file << std::setw(3) << age;
                    std::cout << std::setw(3) << age;
                }
                file << std::endl;
                std::cout << std::endl;
            }
            file << std::endl;
            std::cout << std::endl;
            return true;
        }
    };
}
#endif  // SIMULATION_OUTPUTSYSTEM_H
