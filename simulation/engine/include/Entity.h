#ifndef SIMULATION_ENTITY_H
#define SIMULATION_ENTITY_H

#include <map>
#include <memory>
#include <vector>

namespace Engine {
    struct Entity {
        Entity() = default;
        explicit Entity(std::size_t id) : id(id) {}

        void setSignature(std::size_t componentID);
        [[nodiscard]] const std::vector<std::uint8_t>& getSignature() const;

        template <typename C>
        void setComponent(std::size_t componentID);

        [[nodiscard]] const std::map<std::size_t, std::shared_ptr<Component>>& getComponents() const;
        [[nodiscard]] std::size_t getID() const;

    private:
        std::size_t id {};
        std::vector<std::uint8_t> signature {};
        std::map<std::size_t, std::shared_ptr<Component>> components {};
    };
}  // namespace Engine

#endif  // SIMULATION_ENTITY_H