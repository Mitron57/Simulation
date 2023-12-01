#include <ECS.h>
#include <../100/Solution.h>
//#include <../151/Solution.h>

using namespace ECS;

int main() {
    Solution::prepareForRun();
    Manager::run();
    return 0;
}