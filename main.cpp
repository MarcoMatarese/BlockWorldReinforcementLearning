
#include <iostream>
#include "BlockWorldReinforcementLearner.h"
#include "BlockWorldEnvironment.h"


int main() {
    std::cout << "Hello Marco!" << std::endl;

    int noStates = BlockWorldEnvironment::NO_CONFIGURATIONS,
        noActions = BlockWorldEnvironment::NO_ACTIONS;

    BlockWorldEnvironment env = BlockWorldEnvironment();

    BlockWorldReinforcementLearner robot = BlockWorldReinforcementLearner(noStates, noActions, env);

    robot.TemporalDifferenceRL(100);

    std::cout << "QMATRIX" << std::endl;
    robot.printQMatrix();
    std::cout << "POLICY" << std::endl;
    robot.showPolicy();

    return 0;
}