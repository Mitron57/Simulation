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
            if (field[position->y][position->x] == placeholder) {
                field[y][x] = placeholder;
                field[position->y][position->x] = Herbivor::sign;
            } else {
                position->x = x;
                position->y = y;
            }
        }
        for (auto& predator : predators) {
            const auto [position] {Manager::getComponents<Position>(predator)};
            const auto x = position->x, y = position->y;
            std::uint32_t minDistance {std::numeric_limits<std::uint32_t>::max()
            };
            std::pair coords {0, 0};
            for (std::int32_t y {position->y - 3}; y < position->y + 3; ++y) {
                for (std::int32_t x {position->x - 3}; x < position->x + 3;
                     ++x) {
                    if (0 <= y && y < fieldSize && 0 <= x && x < fieldSize) {
                        if (field[y][x] == Herbivor::sign) {
                            const std::uint32_t distance =
                                std::pow(position->y - y, 2) +
                                std::pow(position->x - x, 2);
                            if (distance < minDistance && distance != 2) {
                                minDistance = distance;
                                coords.first = x;
                                coords.second = y;
                            }
                        }
                    }
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
                if (field[position->y][position->x] != placeholder) {
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
            field[y][x] = placeholder;
            field[position->y][position->x] = Predator::sign;
        }
        return true;
    }
}  // namespace Solution