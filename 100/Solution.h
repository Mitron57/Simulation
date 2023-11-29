#ifndef SIMULATION_SOLUTION_H
#define SIMULATION_SOLUTION_H

#include <Engine.h>

#include <Components.h>
#include <Utils.h>
#include <FieldSystem.h>
#include <InputSystem.h>
#include <LifeSystem.h>
#include <OutputSystem.h>

namespace Solution {
    inline void prepareForRun() {
        const auto field = World::createEntity();
        World::addComponent<Field>(*field);
        World::addSystem<InputSystem>();
        World::addSystem<FieldSystem>();
        World::addSystem<OutputSystem>();
        World::addSystem<LifeSystem>();
    }
}

#endif  // SIMULATION_SOLUTION_H
