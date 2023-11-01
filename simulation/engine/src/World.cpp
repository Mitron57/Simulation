#include <Entity.h>
#include <World.h>

namespace Engine {
    std::map<std::size_t, Entity> World::entities;
    std::vector<std::shared_ptr<System>> World::systems;
    std::map<std::size_t, std::size_t> World::componentsID;
    std::size_t World::componentID;

    std::size_t World::createEntity() {
        static std::size_t entityID {};
        entities.emplace(entityID, entityID);
        return entityID++;
    }

    void World::deleteEntity(std::size_t id) {
        erase_if(entities, [id](auto& entity) { return id == entity.first; });
    }

    template <typename C>
    void World::addComponent(std::size_t entityID) {
        if (!componentsID.contains(typeid(C).hash_code())) {
            componentsID[typeid(C).hash_code()] = componentID++;
        }
        entities[entityID].setComponent<C>(componentsID[typeid(C).hash_code()]);
        entities[entityID].setSignature(componentsID[typeid(C).hash_code()]);
    }

    template <typename S>
    void World::addSystem() {
        systems.push_back(std::make_shared<S>());
    }

    void World::run() {

        for (const auto& system: systems) {
            system->onAwake();
        }
        while (true) {
            for (const auto& system: systems) {
                system->onUpdate();
            }
        }
    }

    template <typename C>
    std::size_t World::getComponentID() {
        return componentsID[typeid(C).hash_code()];
    }

    std::map<std::size_t, Entity>& World::getAllEntities() {
        return entities;
    }

    template <typename C>
    std::vector<std::shared_ptr<Entity>> World::Filter<C>::filter() {
        std::vector<std::shared_ptr<Entity>> requirement {};
        for (auto& entity : entities) {
            auto& entityComponents {entity.second.getSignature()};
            if (entityComponents[componentsID[typeid(C).hash_code()]]) {
                requirement.push_back(std::make_shared<Entity>(entity.second));
            }
        }
        return requirement;
    }

    template <typename C, typename... C2>
    std::vector<std::shared_ptr<Entity>> World::Filter<C, With<C2...>>::filter() {
        std::vector<bool> filter_bits {};
        std::vector<std::shared_ptr<Entity>> requirement {};
        auto to_ulong = [] (auto& vec) {
            std::size_t sum {};
            for (std::size_t i {}; i < vec.size(); ++i) {
                sum += vec[i] ? std::pow(2, i) : 0;
            }
            return sum;
        };
        auto fill_bits = [&filter_bits] (auto type) {
            for (std::size_t i {filter_bits.size()}; i < componentsID[type]; ++i) {
                filter_bits.push_back(false);
            }
            filter_bits.push_back(true);
        };
        fill_bits(typeid(C).hash_code());
        for (auto type : With<C2...>::getTypes()) {
            if (componentsID[type] < filter_bits.size()) {
                filter_bits[componentsID[type]] = true;
            } else {
                fill_bits(type);
            }
        }
        for (auto& entity : entities) {
            auto& entityComponents {entity.second.getSignature()};
            if (to_ulong(filter_bits) == to_ulong(entityComponents)) {
                requirement.push_back(std::make_shared<Entity>(entity.second));
            }
        }
        return requirement;
    }

    template <typename C, typename ...C2>
    std::vector<std::shared_ptr<Entity>> World::Filter<C, Without<C2...>>::filter() {
        std::vector<std::shared_ptr<Entity>> requirement {};
        std::vector<bool> filter_bits {};
        auto to_ulong = [] (auto& vec) {
            std::size_t sum {};
            for (std::size_t i {}; i < vec.size(); ++i) {
                sum += vec[i] ? std::pow(2, i) : 0;
            }
            return sum;
        };
        auto fill_bits = [&filter_bits] (auto type) {
            for (std::size_t i {filter_bits.size()}; i <= componentsID[type]; ++i) {
                filter_bits.push_back(false);
            }
        };
        fill_bits(typeid(C).hash_code());
        filter_bits[componentsID[typeid(C).hash_code()]] = true;
        for (auto type : Without<C2...>::getTypes()) {
            if (componentsID[type] < filter_bits.size()) {
                filter_bits[componentsID[type]] = false;
            } else {
                fill_bits(type);
            }
        }
        for (auto& entity: entities) {
            auto& entityComponents {entity.second.getSignature()};
            if (to_ulong(filter_bits) == to_ulong(entityComponents)) {
                requirement.push_back(std::make_shared<Entity>(entity.second));
            }
        }
        return requirement;
    }
}  // namespace Engine