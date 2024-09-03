#include <SimulationParameters.h>

namespace Solution {
    using namespace SimulationParameters;

    template<typename... T>
    void LifeSystem::generateEntity(std::uint32_t count) {
        std::random_device device {};
        std::mt19937 engine {device()};
        std::uniform_int_distribution<std::uint32_t> random(0, fieldSize - 1);
        for (std::uint32_t i {}; i < count; ++i) {
            std::shared_ptr entity {Manager::createEntity()};
            Manager::addComponent<T...>(entity);
            const auto [position] {Manager::getComponents<Position>(entity)};
            position->x = random(engine);
            position->y = random(engine);
            while (field[position->y][position->x]) {
                position->x = random(engine);
                position->y = random(engine);
            }
            field[position->y][position->x] = entity;
        }
    }

    void LifeSystem::onAwake() {
        generateEntity<Herbivore, Position, Health>(countOfHerbivors);
        generateEntity<Predator, Position, Health>(countOfPredators);
        generateGrass();
    }

    void LifeSystem::generateGrass() {
        std::random_device device {};
        std::mt19937 engine {device()};
        std::uniform_int_distribution<std::uint32_t> randomPosition(
            0, fieldSize - 1
        );
        std::uint32_t count {};
        std::ranges::for_each(field, [&count](const auto& line) {
            count += std::ranges::count_if(line, [] (const std::shared_ptr<Entity>& entity) {
                if (entity) {
                    return entity->getComponents().size() == 1;
                }
                return false;
            });
        });
        for (std::uint32_t i {count}; i < countOfGrass; ++i) {
            if (calculateChance(grassRegeneration)) {
                std::uint32_t x {randomPosition(engine)};
                std::uint32_t y {randomPosition(engine)};
                if (std::ranges::all_of(field, [](const auto& line) {
                        return std::ranges::all_of(line, [](const auto& cell) {
                            return cell != nullptr;
                        });
                    })) {
                    return;
                }
                while (field[y][x]) {
                    x = randomPosition(engine);
                    y = randomPosition(engine);
                }
                std::shared_ptr entity {Manager::createEntity()};
                Manager::addComponent<Grass>(entity);
                field[y][x] = entity;
            }
        }
    }

    template <typename T>
    void LifeSystem::bornChild(std::int32_t posX, std::int32_t posY) {
        for (std::int32_t y {posY - 2}; y <= posY + 2; ++y) {
            for (std::int32_t x {posX - 2}; x <= posX + 2; ++x) {
                if (0 <= y && y < fieldSize && 0 <= x && x < fieldSize) {
                    if (field[y][x] == nullptr) {
                        const auto child = Manager::createEntity();
                        Manager::addComponent<T>(child);
                        Manager::addComponent<Position>(child);
                        Manager::addComponent<Health>(child);
                        const auto [position, health] {
                            Manager::getComponents<Position, Health>(child)
                        };
                        position->y = y;
                        position->x = x;
                        field[y][x] = child;
                        health->birthday = ticks % 12 + 1;
                        return;
                    }
                }
            }
        }
    }

    bool LifeSystem::calculateChance(
        std::double_t chance, std::double_t coeff
    ) {
        std::int32_t power {};
        std::double_t copy {chance};
        while (std::fabs(copy) >= 1e-10) {
            copy *= 10;
            copy -= static_cast<std::int32_t>(copy);
            power++;
        }
        const std::double_t base = std::pow(10, power);
        std::random_device device {};
        std::mt19937 engine {device()};
        std::uniform_int_distribution<std::uint32_t> random(1, base);
        return coeff * random(engine) / base <= chance;
    }

    template <typename T>
    std::tuple<bool, std::int32_t, std::int32_t> LifeSystem::isNear(
        const std::shared_ptr<Position>& position
    ) {
        for (std::int32_t y {position->y - 1}; y <= position->y + 1; ++y) {
            if (0 <= y && y < fieldSize) {
                for (std::int32_t x {position->x - 1}; x <= position->x + 1; ++x) {
                    if (0 <= x && x < fieldSize) {
                        if (std::get<0>(Manager::getComponents<T>(field[y][x])) &&
                            !(x == position->x && y == position->y)) {
                            return {true, x, y};
                        }
                    }
                }
            }
        }
        return {false, 0, 0};
    }

    template <typename T, typename F>
    void LifeSystem::life(std::shared_ptr<Entity>& entity) {
        const auto [position, health] {
            Manager::getComponents<Position, Health>(entity)
        };
        const auto [foodIsNear, x, y] {isNear<F>(position)};
        if (foodIsNear) {
            health->satiety++;
            Manager::deleteEntity(std::move(field[y][x]));
        } else if (health->satiety > 0) {
            health->satiety--;
        }
        if (health->satiety == 0 &&
            calculateChance(deathChance, 1.0 / health->age)) {
            Manager::deleteEntity(std::move(field[position->y][position->x]));
            T::dead++;
            return;
        }
        if (health->age <= maxReproductiveAge &&
            health->age >= minReproductiveAge &&
            std::get<0>(isNear<T>(position)) &&
            calculateChance(birthChance, health->age) &&
            health->satiety > 0) {
            bornChild<T>(position->x, position->y);
        }
        if (ticks % 12 + 1 == health->birthday) {
            health->age++;
        }
        if (health->age > maxAge &&
            calculateChance(deathChance, 1.0 / health->age)) {
            Manager::deleteEntity(std::move(field[position->y][position->x]));
            T::dead++;
        }
    }

    bool LifeSystem::onUpdate() {
        const std::vector herbivores {Filter<Herbivore>::filter()};
        const std::vector predators {Filter<Predator>::filter()};
        countOfHerbivors = herbivores.size();
        if (ticks % 12 == 1) {
            countOfPredators = predators.size();
        }
        if (calculateChance(cataclysmChance)) {
            std::cout << "Cataclysm occurred!" << std::endl;
            return false;
        }
        if (countOfHerbivors == 0 || countOfPredators == 0) {
            std::cout << "END! Species extinction!" << std::endl;
            return false;
        }
        generateGrass();
        for (auto herbivore : herbivores) {
            life<Herbivore, Grass>(herbivore);
        }
        for (auto predator : predators) {
            life<Predator, Herbivore>(predator);
        }
        return ticks < years * 12;
    }
}  // namespace Solution
