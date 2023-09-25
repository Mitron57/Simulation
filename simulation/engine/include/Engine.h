#include <functional>
#include <map>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace Engine {

    struct Engine {
        Engine() {}
        ~Engine() {}

        template <typename... Args>
        using System = std::function<void(Args...)>;

        void add_entity() {}
        void add_component() {}

        template <typename Entity, typename... Args>
        void add_system(Entity& entity, System<Args...> system) {
            Systems<Args...>::systems[entity.name()].push_back(system);
        }

    private:
        template <typename... Args>
        struct Systems {
            static std::unordered_map<std::string, std::vector<System<Args...>>>
                systems;
        };
    };
}  // namespace Engine