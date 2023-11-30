#ifndef SIMULATION_SOLUTION_H
#define SIMULATION_SOLUTION_H

#include <ECS.h>

#include <Components.h>
#include <Utils.h>
#include <FieldSystem.h>
#include <InputSystem.h>
#include <LifeSystem.h>
#include <OutputSystem.h>

namespace Solution {
    inline void prepareForRun() {
        const auto field = Manager::createEntity();
        Manager::addComponent<Field>(field);
        Manager::addSystem<InputSystem>();
        Manager::addSystem<FieldSystem>();
        Manager::addSystem<OutputSystem>();
        Manager::addSystem<LifeSystem>();
    }
}

#endif  // SIMULATION_SOLUTION_H
