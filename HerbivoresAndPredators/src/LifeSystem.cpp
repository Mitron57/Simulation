#include <SimulationParameters.h>

namespace Solution {
    using namespace SimulationParameters;

    void LifeSystem::onAwake() {
        std::random_device device {};
        std::mt19937 engine {device()};
        std::uniform_int_distribution<std::uint32_t> random(0, fieldSize - 1);
        for (std::uint32_t i {}; i < countOfHerbivors; ++i) {
            std::shared_ptr herbivor {Manager::createEntity()};
            Manager::addComponent<Herbivor>(herbivor);
            Manager::addComponent<Health>(herbivor);
            Manager::addComponent<Position>(herbivor);
            const auto [position] {Manager::getComponents<Position>(herbivor)};
            position->x = random(engine);
            position->y = random(engine);
            while (field[position->y][position->x] != placeholder) {
                position->x = random(engine);
                position->y = random(engine);
            }
            field[position->y][position->x] = Herbivor::sign;
        }
        for (std::uint32_t i {}; i < countOfPredators; ++i) {
            std::shared_ptr predator {Manager::createEntity()};
            Manager::addComponent<Predator>(predator);
            Manager::addComponent<Health>(predator);
            Manager::addComponent<Position>(predator);
            const auto [position] {
                Manager::getComponents<Position>(predator)
            };
            position->x = random(engine);
            position->y = random(engine);
            while (field[position->y][position->x] != placeholder) {
                position->x = random(engine);
                position->y = random(engine);
            }
            field[position->y][position->x] = Predator::sign;
        }
        for (std::uint32_t i {}; i < countOfGrass; ++i) {
            std::uint32_t y {random(engine)}, x {random(engine)};
            while (field[y][x] != placeholder) {
                y = random(engine);
                x = random(engine);
            }
            field[y][x] = '#';
        }
    }

    void LifeSystem::generateGrass() {
        std::random_device device {};
        std::mt19937 engine {device()};
        std::uniform_int_distribution<std::uint32_t> randomPosition(
            0, fieldSize - 1
        );
        std::uint32_t count {};
        std::ranges::for_each(field, [&count](auto& line) {
            count += std::ranges::count(line, '#');
        });
        for (std::uint32_t i {count}; i < countOfGrass; ++i) {
            if (calculateChance(grassRegeneration)) {
                std::uint32_t x {randomPosition(engine)};
                std::uint32_t y {randomPosition(engine)};
                if (std::ranges::all_of(field, [](auto& line) {
                        return std::ranges::all_of(line, [](auto cell) {
                            return cell != placeholder;
                        });
                    })) {
                    return;
                }
                while (field[y][x] != placeholder) {
                    x = randomPosition(engine);
                    y = randomPosition(engine);
                }
                field[y][x] = '#';
            }
        }
    }

    template <typename T>
    void LifeSystem::bornChild(std::int32_t posX, std::int32_t posY) {
        for (std::int32_t y {posY - 2}; y < posY + 2; ++y) {
            for (std::int32_t x {posX - 2}; x < posX + 2; ++x) {
                if (0 <= y && y < fieldSize && 0 <= x && x < fieldSize) {
                    if (field[y][x] == placeholder) {
                        const auto child = Manager::createEntity();
                        Manager::addComponent<T>(child);
                        Manager::addComponent<Position>(child);
                        Manager::addComponent<Health>(child);
                        const auto [position, health] {
                            Manager::getComponents<Position, Health>(child)
                        };
                        position->y = y;
                        position->x = x;
                        field[y][x] = T::sign;
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

    std::tuple<bool, std::int32_t, std::int32_t> LifeSystem::isNear(
        char object, const std::shared_ptr<Position>& position
    ) {
        for (std::int32_t y {position->y - 1}; y < position->y + 1; ++y) {
            if (0 <= y && y < fieldSize) {
                for (std::int32_t x {position->x - 1}; x < position->x + 1; ++x) {
                    if (0 <= x && x < fieldSize) {
                        if (field[y][x] == object &&
                            !(x == position->x && y == position->y)) {
                            return {true, x, y};
                        }
                    }
                }
            }
        }
        return {false, 0, 0};
    }

    bool LifeSystem::onUpdate() {
        const std::vector herbivors {Filter<Herbivor>::filter()};
        const std::vector predators {Filter<Predator>::filter()};
        countOfHerbivors = herbivors.size();
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
        std::vector<std::shared_ptr<Entity>> entityToDelete {};
        generateGrass();
        for (auto& herbivor : herbivors) {
            const auto [position, health] {
                Manager::getComponents<Position, Health>(herbivor)
            };
            const auto [foodIsNear, x, y] {isNear('#', position)};
            if (foodIsNear) {
                health->satiety++;
                field[y][x] = placeholder;
            } else if (health->satiety > 0) {
                health->satiety--;
            }
            if (health->satiety == 0 &&
                calculateChance(deathChance, 1.0 / health->age)) {
                field[position->y][position->x] = placeholder;
                entityToDelete.push_back(herbivor);
                deadHerbivors++;
                continue;
            }
            if (health->age <= maxReproductiveAge &&
                health->age >= minReproductiveAge &&
                std::get<0>(isNear(Herbivor::sign, position)) &&
                calculateChance(birthChance, health->age) &&
                health->satiety > 0) {
                bornChild<Herbivor>(position->x, position->y);
            }
            if (ticks % 12 + 1 == health->birthday) {
                health->age++;
            }
            if (health->age > maxAge &&
                calculateChance(deathChance, 1.0 / health->age)) {
                field[position->y][position->x] = placeholder;
                entityToDelete.push_back(herbivor);
                deadHerbivors++;
            }
        }
        for (auto& predator : predators) {
            const auto [position, health] {
                Manager::getComponents<Position, Health>(predator)
            };
            const auto [foodIsNear, x, y] {isNear(Herbivor::sign, position)};
            if (foodIsNear) {
                health->satiety++;
                const auto herbivor {*std::ranges::find_if(
                    herbivors,
                    [x, y](const auto& entity) {
                        const auto [position] {
                            Manager::getComponents<Position>(entity)
                        };
                        return position->x == x && position->y == y;
                    }
                )};
                field[y][x] = placeholder;
                entityToDelete.push_back(herbivor);
                deadHerbivors++;
            } else if (health->satiety > 0) {
                health->satiety--;
            }
            if (ticks % 12 == 1 && health->satiety == 0 &&
                calculateChance(deathChance, 1.0 / health->age)) {
                deadPredators++;
                field[position->y][position->x] = placeholder;
                entityToDelete.push_back(predator);
            }
            if (health->age <= maxReproductiveAge &&
                health->age >= minReproductiveAge &&
                std::get<0>(isNear(Predator::sign, position)) &&
                calculateChance(birthChance, 1.0 / (10 * health->age)) &&
                health->satiety > 0) {
                bornChild<Predator>(position->x, position->y);
            }
            if (ticks % 12 + 1 == health->birthday) {
                health->age++;
            }
            if (health->age > maxAge &&
                calculateChance(deathChance, 1.0 / health->age)) {
                field[position->y][position->x] = placeholder;
                deadPredators++;
                entityToDelete.push_back(predator);
            }
        }
        for (auto& entity : entityToDelete) {
            if (entity) {
                Manager::deleteEntity(std::move(entity));
            }
        }
        return ticks < years * 12;
    }
}  // namespace Solution