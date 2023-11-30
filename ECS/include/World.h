#ifndef SIMULATION_WORLD_H
#define SIMULATION_WORLD_H

#include <map>
#include <unordered_set>

namespace ECS {
    struct Manager {
        using Archetypes =
            std::map<std::size_t, std::vector<std::shared_ptr<Entity>>>;
        static std::shared_ptr<Entity> createEntity();

        static void deleteEntity(std::shared_ptr<Entity> entity);

        template <typename C>
        static void addComponent(const std::shared_ptr<Entity>& entity);

        template <typename S>
        static void addSystem();

        static void run();

        template <typename C>
        static std::size_t getComponentID();
        static std::size_t getComponentID(std::size_t hash);
        template <typename... T>
        static std::tuple<std::shared_ptr<T>...> getComponents(const std::shared_ptr<Entity>& entity);

        static const std::map<std::size_t, std::shared_ptr<Entity>>& getAllEntities();
        static const Archetypes& getArchetypes();

        static void addArchetype(
            const std::vector<std::shared_ptr<Entity>>& archetype
        );

    private:
        static std::map<std::size_t, std::shared_ptr<Entity>> entities;
        static std::vector<std::unique_ptr<System>> systems;
        static std::map<std::size_t, std::size_t> componentsID;
        static Archetypes archetypes;
        static std::size_t componentID;
    };
}  // namespace Engine
#endif  // SIMULATION_WORLD_H
