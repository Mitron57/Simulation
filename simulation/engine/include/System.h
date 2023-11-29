#ifndef SIMULATION_SYSTEM_H
#define SIMULATION_SYSTEM_H

namespace Engine {
    struct System {
        System() = default;
        virtual ~System() = default;

        virtual void onAwake() {}
        virtual bool onUpdate() {
            return true;
        };
    };
}

#endif  // SIMULATION_SYSTEM_H
