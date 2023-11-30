#include <Filter.h>

namespace ECS {
    template <typename C>
    std::vector<std::shared_ptr<Entity>> Filter<C>::filter() {
        const std::uint64_t typeId {Manager::getComponentID<C>()};
        std::vector<std::shared_ptr<Entity>> requirement {};
        const Manager::Archetypes& archetypes = Manager::getArchetypes();
        if (archetypes.contains(typeId)) {
            const auto& archetype {archetypes.at(typeId)};
            for (const auto& [id, entity] : Manager::getAllEntities()) {
                auto& entityComponents {entity->getSignature()};
                if (entityComponents[typeId] != 0 && archetype[archetype.size() - 1]->getID() < id) {
                    requirement.push_back(entity);
                }
            }
            Manager::addArchetype(requirement);
            return Manager::getArchetypes().at(typeId);
        }
        for (const auto& [_, entity] : Manager::getAllEntities()) {
            auto& entityComponents {entity->getSignature()};
            if (entityComponents[typeId] != 0) {
                requirement.push_back(entity);
            }
        }
        Manager::addArchetype(requirement);
        return requirement;
    }

    template <typename C, typename... C2>
    std::vector<std::shared_ptr<Entity>> Filter<C, With<C2...>>::filter() {
        using namespace Utils;
        std::vector<std::uint8_t> filterBits {};
        std::vector<std::shared_ptr<Entity>> requirement {};
        fillMissingBits(filterBits, Manager::getComponentID<C>());
        filterBits.push_back(1);
        for (auto hash : With<C2...>::getTypeHash()) {
            auto componentId = Manager::getComponentID(hash);
            if (filterBits.size() <= componentId) {
                fillMissingBits(filterBits, componentId);
                filterBits.push_back(1);
            } else {
                filterBits[componentId] = 1;
            }
        }
        std::uint64_t typesId {bitSequenceToULL(filterBits)};
        const Manager::Archetypes& archetypes = Manager::getArchetypes();
        if (archetypes.contains(typesId)) {
            const auto& archetype {archetypes.at(typesId)};
            for (const auto& [id, entity] : Manager::getAllEntities()) {
                if (archetype.empty() || archetype[archetype.size() - 1]->getID() < id) {
                    const std::uint64_t signature {
                        bitSequenceToULL(entity->getSignature())
                    };
                    if ((typesId & signature) == typesId) {
                        requirement.push_back(entity);
                    }
                }
            }
            Manager::addArchetype(requirement);
            return Manager::getArchetypes().at(typesId);
        }
        for (auto& entity : Manager::getAllEntities()) {
            const std::uint64_t signature {
                bitSequenceToULL(entity.second->getSignature())
            };
            if ((typesId & signature) == typesId) {
                requirement.push_back(entity.second);
            }
        }
        Manager::addArchetype(requirement);
        return requirement;
    }

    template <typename C, typename... C2>
    std::vector<std::shared_ptr<Entity>> Filter<C, Without<C2...>>::filter() {
        using namespace Utils;
        std::vector<std::shared_ptr<Entity>> requirement {};
        std::vector<std::uint8_t> filterBits {};
        const Manager::Archetypes& archetypes = Manager::getArchetypes();
        fillMissingBits(filterBits, Manager::getComponentID<C>());
        filterBits.push_back(1);
        const std::size_t typeId {bitSequenceToULL(filterBits)};
        if (archetypes.contains(typeId)) {
            const auto& archetype {archetypes.at(typeId)};
            for (const auto& [id, entity] : Manager::getAllEntities()) {
                if (archetype.empty() || archetype[archetype.size() - 1]->getID() < id) {
                    const std::uint64_t signature {
                        bitSequenceToULL(entity->getSignature())
                    };
                    if ((typeId & signature) == signature) {
                        requirement.push_back(entity);
                    }
                }
            }
            Manager::addArchetype(requirement);
            return Manager::getArchetypes().at(typeId);
        }
        for (auto& entity : Manager::getAllEntities()) {
            const std::uint64_t signature {
                bitSequenceToULL(entity.second->getSignature())
            };
            if ((typeId & signature) == signature) {
                requirement.push_back(entity.second);
            }
        }
        Manager::addArchetype(requirement);
        return requirement;
    }
}  // namespace Engine