#include <sys/ioctl.h>
#include <unistd.h>

#include <iostream>

namespace Engine {

    struct DrawEngineBase {
        DrawEngineBase() {
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &this->w);
        }

        ~DrawEngineBase() = default;

        virtual void draw() = 0;

    private:
        winsize w;
    };

}  // namespace Engine