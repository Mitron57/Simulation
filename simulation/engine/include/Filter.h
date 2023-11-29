#ifndef SIMULATION_FILTER_H
#define SIMULATION_FILTER_H

namespace Engine {
    template <typename...>
    struct Filter {
        static std::vector<std::shared_ptr<Entity>> filter();
    };

    template <typename C>
    struct Filter<C> {
        Filter() = delete;
        static std::vector<std::shared_ptr<Entity>> filter();
    };

    template <typename C, typename... C2>
    struct Filter<C, With<C2...>> {
        Filter() = delete;
        static std::vector<std::shared_ptr<Entity>> filter();
    };

    template <typename C, typename... C2>
    struct Filter<C, Without<C2...>> {
        Filter() = delete;
        static std::vector<std::shared_ptr<Entity>> filter();
    };
} // namespace Engine

#endif  // SIMULATION_FILTER_H
