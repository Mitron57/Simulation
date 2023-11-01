#include <Engine.h>

#include <csignal>
#include <iostream>
#include <thread>
using namespace Engine;

struct Signal : Component {};

struct Transform : Component {
    std::int32_t positionX {};
    std::int32_t positionY {};
};

struct Movement : Component {
    std::int32_t velocityX {};
    std::int32_t velocityY {};
};

struct TransformMovementSystem : System {
    void onUpdate() override {
        auto entities = World::Filter<Transform>::filter();
        for (auto& entity : entities) {
            std::cout << "Entity id: " << entity->getID() << std::endl;
            for (auto& component : entity->getComponents()) {
                if (component.first == World::getComponentID<Transform>()) {
                    auto pTransform =
                        reinterpret_pointer_cast<Transform>(component.second);
                    std::cout << "Transform: " << (pTransform->positionX += 10)
                              << " " << (pTransform->positionY += 10)
                              << std::endl;
                    std::this_thread::sleep_for(
                        std::chrono::nanoseconds {10000000});
                }
            }
        }
    }

    void onAwake() override {}
};

struct SignalSystem : System {
    void onAwake() override {
        std::signal(SIGINT, handler);
        std::signal(SIGTERM, handler);
    }

    void onUpdate() override {}

    static void handler(std::int32_t signal) {
        if (signal == SIGINT || signal == SIGTERM) {
            system("clear");
            auto allEntities = World::getAllEntities();
            for (auto& entity : allEntities) {
                std::cout << "EntityID: " << entity.first << std::endl;
                std::cout << "Conponents:" << std::endl;
                for (auto& component : entity.second.getComponents()) {
                    if (component.first == World::getComponentID<Transform>()) {
                        auto pTransform = reinterpret_pointer_cast<Transform>(
                            component.second);
                        std::cout << "\tTransform: " << pTransform->positionX
                                  << " " << pTransform->positionY << std::endl;
                    } else if (component.first ==
                               World::getComponentID<Movement>()) {
                        auto pMovement = reinterpret_pointer_cast<Movement>(
                            component.second);
                        std::cout << "\tMovement: " << pMovement->velocityX
                                  << " " << pMovement->velocityY << std::endl;
                    }
                }
            }
            std::exit(signal);
        }
    }
};

int main() {
    std::size_t grass = World::createEntity();
    std::size_t unit = World::createEntity();
    std::size_t signal = World::createEntity();
    World::addComponent<Transform>(grass);
    World::addComponent<Transform>(unit);
    World::addComponent<Movement>(unit);
    World::addComponent<Signal>(signal);
    World::addSystem<SignalSystem>();
    World::addSystem<TransformMovementSystem>();
    World::run();
    return 0;
}