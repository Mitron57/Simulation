#include <Engine.h>

#include "../100/Solution.h"

using namespace Engine;

int main() {
    Solution::prepareForRun();
    World::run();
    return 0;
}