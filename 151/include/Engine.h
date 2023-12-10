#ifndef ENGINE_H
#define ENGINE_H
namespace Solution {
    struct Engine {
        Engine();
        void run();

    private:
        static void readConfig();
        static void signalHandler(std::int32_t);
        static void endGame();
        MovementSystem movement;
        LifeSystem life;
#ifdef WIN32
        static CONSOLE_SCREEN_BUFFER_INFO csbi;
        static std::int32_t rows;
#else
        static winsize window;
#endif
    };
}
#endif  // ENGINE_H
