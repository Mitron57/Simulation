#ifndef SOLUTION_H
#define SOLUTION_H

#include <ECS.h>
#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <csignal>
#include <cstdint>
#ifdef WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif


namespace Solution {
    using Field = std::vector<std::vector<std::shared_ptr<ECS::Entity>>>;
}

#include <SimulationParameters.h>
#include <Components.h>
#include <MovementSystem.h>
#include <MovementSystem.cpp>
#include <LifeSystem.h>
#include <LifeSystem.cpp>
#include <Engine.h>
#include <Engine.cpp>

#endif //SOLUTION_H
