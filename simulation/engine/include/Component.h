#ifndef SIMULATION_COMPONENT_H
#define SIMULATION_COMPONENT_H

#include <bitset>

namespace Engine {
    struct Component {
        Component() = default;
        Component(const Component& component) = default;
        Component(Component&& component) = default;

        virtual ~Component() = default;
    };
}

#endif  // SIMULATION_COMPONENT_H
