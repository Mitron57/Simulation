#ifndef SIMULATION_INPUTSYSTEM_H
#define SIMULATION_INPUTSYSTEM_H

#include <algorithm>

using namespace ECS;

namespace Solution {
    struct InputSystem final : System {
        void onAwake() override {
            std::string field {}, letters {}, generation {};
            std::cout << "Enter size of field:" << std::endl << ">> ";
            fieldSize = read(field);
            std::cout << "Enter count of letters:" << std::endl << ">> ";
            lettersCount = read(letters);
            std::cout << "Enter count of generations:" << std::endl << ">> ";
            maxGeneration = read(generation);
        }

        std::uint32_t read (std::string& number) {
            while (!std::getline(std::cin, number)) {
                std::cerr << "Unexpected error occurred, enter number:"
                          << std::endl;
                std::cout << ">> ";
            }
            while ((number[0] == '0' && number.length() > 1) ||
                   std::ranges::any_of(number.begin(), number.end(), [](char elem) {
                       return !('0' <= elem && elem <= '9');}
                   )) {
                std::cerr << "Number can't contain letters or begin with 0, enter number:" << std::endl;
                std::cout << ">> ";
                read(number);
            }
            return std::stoi(number);
        }
    };
}  // namespace Solution
#endif  // SIMULATION_INPUTSYSTEM_H
