
#include <iostream>
#include "BlockWorldReinforcementLearner.h"
#include "BlockWorldEnvironment.h"


// TODO metodo di fine learning basato sull'errore quadratico medio: va a zero spesso... trovare un soluzione!
// TODO rendere l'ambiente dinamico: n cubi
// TODO frequenza scelta dell'azione giusta


int main() {
    std::cout << "Hello Marco!" << std::endl;

    int noStates = BlockWorldEnvironment::NO_CONFIGURATIONS,
        noActions = BlockWorldEnvironment::NO_ACTIONS,
        *noActionsToWin = nullptr;

    static int optimalPolicy[22] = {1, 0, 2, 0, 2, 0, 1, 9, 6, 11, 3, 9, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11};

    bool isOptimal = true;

    BlockWorldEnvironment env = BlockWorldEnvironment();

    BlockWorldReinforcementLearner robot = BlockWorldReinforcementLearner(noStates, noActions, env);

    robot.TemporalDifferenceRL(100, optimalPolicy);
    robot.calculatePolicy();

    robot.showPolicy();

    noActionsToWin = robot.getNoActionsToWin();

    //std::cout << "QMATRIX" << std::endl;
    //robot.printQMatrix();
    //std::cout << "POLICY" << std::endl;
    //robot.showPolicy();

    // check if policy is optimal
    for(int i = 0; i < noStates; i++) {
        if (robot.getPolicyValue(i) != optimalPolicy[i]) {
            isOptimal = false;
            break;
        }
    }
    if(isOptimal) std::cout << "Policy is optimal." << std::endl;
    else std::cout << "Policy is NOT optimal." << std::endl;

    // print no actions to do to achieve accepting configuration
    std::cout << std::endl << "-------------------------" << std::endl;
    std::cout << "  STATE    | NO ACTIONS  " << std::endl;
    std::cout << "-------------------------" << std::endl;
    for(int i = 0; i < noStates; i++)
        std::cout << "  " << i << "             " << noActionsToWin[i] << std::endl;

    return 0;
}