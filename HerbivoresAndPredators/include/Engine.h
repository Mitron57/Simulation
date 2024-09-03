#ifndef ENGINE_H
#define ENGINE_H
namespace Solution {
    struct Engine {
        Engine();
        void run();

    private:
        static void printField();
        static void readConfig();
        static void signalHandler(std::int32_t);
        static void endGame();
        static void printConfig();
        MovementSystem movement;
        LifeSystem life;
        static std::int32_t rows;
    };
}
#endif  // ENGINE_H
