#ifndef SIMULATION_QUERY_H
#define SIMULATION_QUERY_H

namespace Engine {
    template <typename... C>
    struct Query {
        [[nodiscard]] static std::vector<std::size_t> getTypeHash() {
            return {typeid(C).hash_code()...};
        }
    };

    template <typename... C>
    struct With : Query<C...> {};

    template <typename... C>
    struct Without : Query<C...> {};
}  // namespace Engine

#endif  // SIMULATION_QUERY_H
