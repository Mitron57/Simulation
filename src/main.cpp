#include <ECS.h>

#include "../100/Solution.h"

using namespace ECS;

int main() {
    Solution::prepareForRun();
    Manager::run();
    return 0;
}