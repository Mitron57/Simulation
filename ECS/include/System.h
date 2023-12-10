#ifndef SIMULATION_SYSTEM_H
#define SIMULATION_SYSTEM_H

namespace ECS {
    struct System {
        System() = default;
        virtual ~System() = default;

        virtual void onAwake() = 0;
        virtual bool onUpdate() = 0;
    };
}

#endif  // SIMULATION_SYSTEM_H
