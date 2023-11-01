#include <Entity.h>

namespace Engine {
    void Entity::setSignature(std::size_t componentID) {
        auto size = signature.size();
        for (std::size_t i {size}; i < componentID; ++i) {
            signature.push_back(false);
        }
        signature.push_back(true);
    }

    std::vector<bool>& Entity::getSignature() {
        return signature;
    }

    template <typename C>
    void Entity::setComponent(std::size_t componentID) {
        components[componentID] = std::make_shared<C>();
    }

    std::map<std::size_t, std::shared_ptr<Component>>& Entity::getComponents() {
        return components;
    }
    std::size_t Entity::getID() const {
        return this->id;
    }
}  // namespace Engine