#ifndef SIMULATION_ENTITY_H
#define SIMULATION_ENTITY_H

#include <any>
#include <map>

namespace Engine {
    struct Entity {
        Entity() = default;
        Entity(std::size_t id) : id(id) {}

        void setSignature(std::size_t componentID);
        std::vector<bool>& getSignature();

        template<typename C>
        void setComponent(std::size_t componentID);

        std::map<std::size_t, std::shared_ptr<Component>>& getComponents();
        [[nodiscard]] std::size_t getID() const;
    private:
        std::size_t id {};
        std::vector<bool> signature {};
        std::map<std::size_t, std::shared_ptr<Component>> components {};
    };
}  // namespace Engine

#endif  // SIMULATION_ENTITY_H