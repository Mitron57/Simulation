#include <Entity.h>

namespace Engine {
    void Entity::setSignature(std::size_t componentID) {
        if (std::size_t size = signature.size(); componentID >= size) {
            for (std::size_t i {size}; i < componentID; ++i) {
                signature.push_back(0);
            }
            signature.push_back(1);
            return;
        }
        signature[componentID] = 1;
    }

    const std::vector<std::uint8_t>& Entity::getSignature() const {
        return signature;
    }

    template <typename C>
    void Entity::setComponent(std::size_t componentID) {
        components[componentID] = std::make_shared<C>();
    }

    const std::map<std::size_t, std::shared_ptr<Component>>& Entity::getComponents() const {
        return components;
    }
    std::size_t Entity::getID() const {
        return this->id;
    }
}  // namespace Engine