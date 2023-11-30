#include <World.h>

namespace ECS {
    std::map<std::size_t, std::shared_ptr<Entity>> Manager::entities;
    std::vector<std::unique_ptr<System>> Manager::systems;
    std::map<std::size_t, std::size_t> Manager::componentsID;
    Manager::Archetypes Manager::archetypes;
    std::size_t Manager::componentID;

    std::shared_ptr<Entity> Manager::createEntity() {
        static std::size_t entityID {};
        entities.emplace(entityID, std::make_shared<Entity>(entityID));
        return entities.at(entityID++);
    }

    void Manager::deleteEntity(const Entity& entity) {
        entities.erase(entity.getID());
        std::erase_if(
            archetypes[Utils::bitSequenceToULL(entity.getSignature())],
            [id = entity.getID()](auto entity) { return id == entity->getID(); }
        );
    }
    template <typename C>
    void Manager::addComponent(Entity& entity) {
        if (!componentsID.contains(typeid(C).hash_code())) {
            componentsID[typeid(C).hash_code()] = componentID++;
        }
        entity.setComponent<C>(componentsID[typeid(C).hash_code()]);
        entity.setSignature(componentsID[typeid(C).hash_code()]);
    }

    template <typename S>
    void Manager::addSystem() {
        systems.push_back(std::make_unique<S>());
    }

    void Manager::run() {
        for (const auto& system : systems) {
            system->onAwake();
        }
        while (true) {
            for (const auto& system : systems) {
                if (!system->onUpdate()) {
                    return;
                }
            }
        }
    }

    template <typename C>
    std::size_t Manager::getComponentID() {
        return componentsID[typeid(C).hash_code()];
    }

    std::size_t Manager::getComponentID(std::size_t hash) {
        return componentsID[hash];
    }

    template <typename... T>
    std::tuple<std::shared_ptr<T>...> Manager::getComponents(Entity& entity) {
        auto& components {entity.getComponents()};
        return {reinterpret_pointer_cast<T>(
            components.at(Manager::getComponentID<T>())
        )...};
    }

    const std::map<std::size_t, std::shared_ptr<Entity>>& Manager::getAllEntities(
    ) {
        return entities;
    }

    const Manager::Archetypes& Manager::getArchetypes() {
        return archetypes;
    }

    void Manager::addArchetype(
        const std::vector<std::shared_ptr<Entity>>& archetype
    ) {
        if (!archetype.empty()) {
            const std::size_t signature {
                Utils::bitSequenceToULL(archetype[0]->getSignature())
            };
            if (archetypes.contains(signature)) {
                archetypes[signature].insert(
                    archetypes[signature].end(), archetype.begin(),
                    archetype.end()
                );
                return;
            }
            archetypes[signature] = archetype;
        }
    }

}  // namespace Engine