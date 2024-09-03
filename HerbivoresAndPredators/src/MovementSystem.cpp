namespace Solution {
    using namespace SimulationParameters;

    template <typename T>
    void MovementSystem::moveEntity(const std::shared_ptr<Position>& position) {
        std::random_device device {};
        std::mt19937 engine {device()};
        const std::int32_t x {position->x}, y {position->y};
        std::uint32_t minDistance {std::numeric_limits<std::uint32_t>::max()};
        std::pair coords {
            std::numeric_limits<std::int32_t>::max(),
            std::numeric_limits<std::int32_t>::max()
        };
        bool broke = false;
        for (std::int32_t posY {position->y - 3}; posY <= position->y + 3; ++posY) {
            if (0 <= posY && posY < fieldSize) {
                for (std::int32_t posX {position->x - 3}; posX <= position->x + 3; ++posX) {
                    if (0 <= posX && posX < fieldSize) {
                        if (Manager::hasComponent<T>(field[posY][posX])) {
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
            if (Manager::hasComponent<Herbivore>(field[position->y][position->x]) ||
                Manager::hasComponent<Predator>(field[position->y][position->x])
            ) {
                position->x = x;
                position->y = y;
            }
        } else if (!broke) {
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
        if (Manager::hasComponent<Grass>(field[position->y][position->x])) {
            Manager::deleteEntity(std::move(field[position->y][position->x]));
            std::swap(field[y][x], field[position->y][position->x]);
            return;
        }
        if (!field[position->y][position->x]) {
            std::swap(field[y][x], field[position->y][position->x]);
            return;
        }
        position->x = x;
        position->y = y;
    }

    bool MovementSystem::onUpdate() {
        const std::vector herbivores {Filter<Herbivore>::filter()};
        const std::vector predators {Filter<Predator>::filter()};
        for (auto& herbivore : herbivores) {
            const auto [position] {Manager::getComponents<Position>(herbivore)};
            moveEntity<Grass>(position);
        }
        for (auto& predator : predators) {
            const auto [position] {Manager::getComponents<Position>(predator)};
            moveEntity<Herbivore>(position);
        }
        return true;
    }
}  // namespace Solution
