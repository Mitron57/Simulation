#include <ECS.h>
#include <Solution.h>

using namespace ECS;

int main() {
    //Solution::prepareForRun();
    Solution::Engine engine {};
    engine.run();
    //Manager::run();
    return 0;
}