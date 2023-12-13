#ifndef SIMULATIONPARAMETERS_H
#define SIMULATIONPARAMETERS_H

namespace Solution::SimulationParameters {
    static Field field;
    static std::uint64_t ticks {};
    static std::uint32_t years {};
    static std::uint32_t fieldSize {};
    static std::uint32_t maxAge {};
    static std::uint32_t maxReproductiveAge {};
    static std::uint32_t minReproductiveAge {};
    static std::uint32_t countOfPredators {};
    static std::uint32_t countOfHerbivors {};
    static std::uint32_t countOfGrass {};
    static std::double_t grassRegeneration {};
    static std::double_t cataclysmChance {};
    static std::double_t deathChance {};
    static std::double_t birthChance {};
    static std::uint32_t deadHerbivors {};
    static std::uint32_t deadPredators {};
    static char placeholder {};
}  // namespace Solution::SimulationParameters

#endif  // SIMULATIONPARAMETERS_H
