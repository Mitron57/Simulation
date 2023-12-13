#include <Manager.h>

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

    void Manager::deleteEntity(std::shared_ptr<Entity> entity) {
        entities.erase(entity->getID());
        auto first {
            archetypes[Utils::bitSequenceToULL(entity->getSignature())].begin()
        };
        const auto end {
            archetypes[Utils::bitSequenceToULL(entity->getSignature())].end()
        };
        for (; first != end; ++first) {
            if ((*first)->getID() == entity->getID()) {
                archetypes[Utils::bitSequenceToULL(entity->getSignature())]
                    .erase(first);
                break;
            }
        }
    }
    template <typename C>
    void Manager::addComponent(const std::shared_ptr<Entity>& entity) {
        if (entity) {
            if (!componentsID.contains(typeid(C).hash_code())) {
                componentsID[typeid(C).hash_code()] = componentID++;
            }
            entity->setComponent<C>(componentsID[typeid(C).hash_code()]);
        }
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
    std::tuple<std::shared_ptr<T>...> Manager::getComponents(
        const std::shared_ptr<Entity>& entity
    ) {
        auto& components {entity->getComponents()};
        return {std::reinterpret_pointer_cast<T>(
            components.at(Manager::getComponentID<T>())
        )...};
    }

    const std::map<std::size_t, std::shared_ptr<Entity>>&
    Manager::getAllEntities() {
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
                for (auto& entity : archetype) {
                    auto iter {std::ranges::find_if(archetypes[signature], [id = entity->getID()] (const auto& unique) {
                        return unique->getID() == id;
                    })};
                    if (iter == archetypes[signature].end()) {
                        archetypes[signature].push_back(entity);
                    }
                }
                return;
            }
            archetypes[signature] = archetype;
        }
    }

}  // namespace ECS