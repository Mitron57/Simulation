namespace Solution {
    using namespace SimulationParameters;
    bool MovementSystem::onUpdate() {
        std::random_device device {};
        std::mt19937 engine {device()};
        const std::vector herbivors {Filter<Herbivore>::filter()};
        const std::vector predators {Filter<Predator>::filter()};
        for (auto& herbivor : herbivors) {
            const auto [position] {Manager::getComponents<Position>(herbivor)};
            const std::int32_t x {position->x}, y {position->y};
            std::uniform_int_distribution randomX(
                position->x == 0 ? 0 : position->x - 1,
                position->x == fieldSize - 1 ? position->x : position->x + 1
            );
            std::uniform_int_distribution randomY(
                position->y == 0 ? 0 : position->y - 1,
                position->y == fieldSize - 1 ? position->y : position->y + 1
            );
            position->x = randomX(engine);
            position->y = randomY(engine);
            if (field[position->y][position->x] == Herbivore::sign ||
                field[position->y][position->x] == Predator::sign) {
                position->x = x;
                position->y = y;
            }
            field[y][x] = placeholder;
            field[position->y][position->x] = Herbivore::sign;
        }
        for (auto& predator : predators) {
            const auto [position] {Manager::getComponents<Position>(predator)};
            const std::int32_t x {position->x}, y {position->y};
            field[y][x] = placeholder;
            std::uint32_t minDistance {std::numeric_limits<std::uint32_t>::max()
            };
            std::pair coords {
                std::numeric_limits<std::int32_t>::max(),
                std::numeric_limits<std::int32_t>::max()
            };
            bool broke = false;
            for (std::int32_t posY {position->y - 3}; posY < position->y + 3;
                 ++posY) {
                if (0 <= posY && posY < fieldSize) {
                    for (std::int32_t posX {position->x - 3};
                         posX < position->x + 3; ++posX) {
                        if (0 <= posX && posX < fieldSize) {
                            if (field[posY][posX] == Herbivore::sign) {
                                const std::uint32_t distance =
                                    std::pow(position->y - posY, 2) +
                                    std::pow(position->x - posX, 2);
                                if (distance <= 2) {
                                    coords = {posX, posY};
                                    broke = true;
                                    break;
                                }
                                if (distance < minDistance) {
                                    minDistance = distance;
                                    coords = {posX, posY};
                                }
                            }
                        }
                    }
                }
                if (broke) {
                    break;
                }
            }
            if (coords == std::pair {
                              std::numeric_limits<std::int32_t>::max(),
                              std::numeric_limits<std::int32_t>::max()
                          }) {
                std::uniform_int_distribution randomX(
                    position->x == 0 ? 0 : position->x - 1,
                    position->x == fieldSize - 1 ? position->x : position->x + 1
                );
                std::uniform_int_distribution randomY(
                    position->y == 0 ? 0 : position->y - 1,
                    position->y == fieldSize - 1 ? position->y : position->y + 1
                );
                position->x = randomX(engine);
                position->y = randomY(engine);
                if (field[position->y][position->x] == Herbivore::sign ||
                    field[position->y][position->x] == Predator::sign) {
                    position->x = x;
                    position->y = y;
                }
            } else {
                if (coords.first > position->x) {
                    position->x++;
                } else if (coords.first < position->x) {
                    position->x--;
                }
                if (coords.second > position->y) {
                    position->y++;
                } else if (coords.second < position->y) {
                    position->y--;
                }
            }
            field[position->y][position->x] = Predator::sign;
        }
        return true;
    }
}  // namespace Solution