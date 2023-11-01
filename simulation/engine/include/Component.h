#ifndef SIMULATION_COMPONENT_H
#define SIMULATION_COMPONENT_H

namespace Engine {
    struct Component {
        Component() = default;
        Component(const Component& component) = default;
        Component(Component&& component) = default;

        virtual ~Component() = default;
    };
}

#endif  // SIMULATION_COMPONENT_H
