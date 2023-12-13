namespace Solution {
    using namespace SimulationParameters;
    bool MovementSystem::onUpdate() {
        std::random_device device {};
        std::mt19937 engine {device()};
        const std::vector herbivors {Filter<Herbivor>::filter()};
        const std::vector predators {Filter<Predator>::filter()};
        for (auto& herbivor : herbivors) {
            const auto [position] {Manager::getComponents<Position>(herbivor)};
            const auto x = position->x, y = position->y;
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
            if (field[position->y][position->x] == Herbivor::sign ||
                field[position->y][position->x] == Predator::sign) {
                position->x = x;
                position->y = y;
            }
            field[y][x] = placeholder;
            field[position->y][position->x] = Herbivor::sign;
        }
        for (auto& predator : predators) {
            const auto [position] {Manager::getComponents<Position>(predator)};
            const auto x = position->x, y = position->y;
            field[y][x] = placeholder;
            std::uint32_t minDistance {std::numeric_limits<std::uint32_t>::max()
            };
            std::pair coords {0, 0};
            bool broke = false;
            for (std::int32_t posY {position->y - 3}; posY < position->y + 3; ++posY) {
                for (std::int32_t posX {position->x - 3}; posX < position->x + 3;
                     ++posX) {
                    if (0 <= posY && posY < fieldSize && 0 <= posX && posX < fieldSize) {
                        if (field[posY][posX] == Herbivor::sign) {
                            const std::uint32_t distance =
                                std::pow(position->y - posY, 2) +
                                std::pow(position->x - posX, 2);
                            if (distance <= 2) {
                                coords = {posX, posY};
                                broke = true;
                                break;
                            }
                            if (distance < minDistance && distance > 2) {
                                minDistance = distance;
                                coords.first = posX;
                                coords.second = posY;
                            }
                        }
                    }
                }
                if (broke) {
                    break;
                }
            }
            if (coords == std::pair {0, 0}) {
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
                if (field[position->y][position->x] == Herbivor::sign ||
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