#ifndef SIMULATION_WORLD_H
#define SIMULATION_WORLD_H

#include <algorithm>
#include <map>

namespace Engine {
    struct World {
        World() = default;

        static std::size_t createEntity();

        static void deleteEntity(std::size_t id);

        template <typename C>
        static void addComponent(std::size_t entityID);

        template <typename S>
        static void addSystem();

        static void run();

        template <typename C>
        static std::size_t getComponentID();

        static std::map<std::size_t, Entity>& getAllEntities();

        template <typename...>
        struct Filter;

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

    private:
        static std::map<std::size_t, Entity> entities;
        static std::vector<std::shared_ptr<System>> systems;
        static std::map<std::size_t, std::size_t> componentsID;
        static std::size_t componentID;
    };
}  // namespace Engine
#endif  // SIMULATION_WORLD_H
