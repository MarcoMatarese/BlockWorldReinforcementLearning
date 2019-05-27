//
// Created by Marco Matarese on 15/05/2019.
//


#include <iostream>
#include "BlockWorldEnvironment.h"


/**
 * Initialize the transition matrix.
 * It defines a function f : (s, a)-->s' where s, s' belong to S and a belong to A.
 */
void BlockWorldEnvironment::initTransitionFunction() {

    for(int i = 0; i < NO_CONFIGURATIONS; i++)
        for(int j = 0; j < NO_ACTIONS; j++)
            this->transitionFunction[i][j] = -1;

    this->transitionFunction[0][0] = 7;
    this->transitionFunction[0][1] = 8;
    this->transitionFunction[0][2] = 9;

    this->transitionFunction[1][0] = 10;
    this->transitionFunction[1][1] = 8;

    this->transitionFunction[2][0] = 11;
    this->transitionFunction[2][2] = 9;

    this->transitionFunction[3][0] = 7;
    this->transitionFunction[3][1] = 12;

    this->transitionFunction[4][1] = 13;
    this->transitionFunction[2][2] = 9;

    this->transitionFunction[5][0] = 7;
    this->transitionFunction[5][2] = 14;

    this->transitionFunction[6][1] = 8;
    this->transitionFunction[6][2] = 15;

    this->transitionFunction[7][3] = 5;
    this->transitionFunction[7][4] = 3;
    this->transitionFunction[7][9] = 0;

    this->transitionFunction[8][5] = 6;
    this->transitionFunction[8][6] = 1;
    this->transitionFunction[8][10] = 0;

    this->transitionFunction[9][7] = 4;
    this->transitionFunction[9][8] = 2;
    this->transitionFunction[9][11] = 0;

    this->transitionFunction[10][3] = 21;
    this->transitionFunction[10][9] = 1;

    this->transitionFunction[11][4] = 17;
    this->transitionFunction[11][9] = 2;

    this->transitionFunction[12][5] = 18;
    this->transitionFunction[12][10] = 3;

    this->transitionFunction[13][6] = 19;
    this->transitionFunction[13][10] = 4;

    this->transitionFunction[14][7] = 20;
    this->transitionFunction[14][11] = 5;

    this->transitionFunction[15][8] = 16;
    this->transitionFunction[15][11] = 6;
}

/**
* Get the possible actions depending on the current world's configuration (defined by this->availableActionInN arrays).
* @return a vector containing the available actions in the current world configuration.
*/
std::vector<int> BlockWorldEnvironment::getAvailableActionsFromCurrentState() {

    return getAvailableActionsFromState(this->currConfiguration);
}

/**
 * Get the possible actions depending on the passed world's configuration.
 * @param state the world's configuration.
 * @return a vector containing the available actions in the current world configuration.
 */
std::vector<int> BlockWorldEnvironment::getAvailableActionsFromState(int state) {
    std::vector<int> availableActions = std::vector<int>();

    switch(state) {
        case 0:
            availableActions = std::vector<int>(this->availableActionIn0,
                                                this->availableActionIn0 + sizeof(this->availableActionIn0) / sizeof(int));
            break;
        case 1:
            availableActions = std::vector<int>(this->availableActionIn1,
                                                this->availableActionIn1 + sizeof(this->availableActionIn1) / sizeof(int));
            break;
        case 2:
            availableActions = std::vector<int>(this->availableActionIn2,
                                                this->availableActionIn2 + sizeof(this->availableActionIn2) / sizeof(int));
            break;
        case 3:
            availableActions = std::vector<int>(this->availableActionIn3,
                                                this->availableActionIn3 + sizeof(this->availableActionIn3) / sizeof(int));
            break;
        case 4:
            availableActions = std::vector<int>(this->availableActionIn4,
                                                this->availableActionIn4 + sizeof(this->availableActionIn4) / sizeof(int));
            break;
        case 5:
            availableActions = std::vector<int>(this->availableActionIn5,
                                                this->availableActionIn5 + sizeof(this->availableActionIn5) / sizeof(int));
            break;
        case 6:
            availableActions = std::vector<int>(this->availableActionIn6,
                                                this->availableActionIn6 + sizeof(this->availableActionIn6) / sizeof(int));
            break;
        case 7:
            availableActions = std::vector<int>(this->availableActionIn7,
                                                this->availableActionIn7 + sizeof(this->availableActionIn7) / sizeof(int));
            break;
        case 8:
            availableActions = std::vector<int>(this->availableActionIn8,
                                                this->availableActionIn8 + sizeof(this->availableActionIn8) / sizeof(int));
            break;
        case 9:
            availableActions = std::vector<int>(this->availableActionIn9,
                                                this->availableActionIn9 + sizeof(this->availableActionIn9) / sizeof(int));
            break;
        case 10:
            availableActions = std::vector<int>(this->availableActionIn10,
                                                this->availableActionIn10 + sizeof(this->availableActionIn10) / sizeof(int));
            break;
        case 11:
            availableActions = std::vector<int>(this->availableActionIn11,
                                                this->availableActionIn11 + sizeof(this->availableActionIn11) / sizeof(int));
            break;
        case 12:
            availableActions = std::vector<int>(this->availableActionIn12,
                                                this->availableActionIn12 + sizeof(this->availableActionIn12) / sizeof(int));
            break;
        case 13:
            availableActions = std::vector<int>(this->availableActionIn13,
                                                this->availableActionIn13 + sizeof(this->availableActionIn13) / sizeof(int));
            break;
        case 14:
            availableActions = std::vector<int>(this->availableActionIn14,
                                                this->availableActionIn14 + sizeof(this->availableActionIn14) / sizeof(int));
            break;
        case 15:
            availableActions = std::vector<int>(this->availableActionIn15,
                                                this->availableActionIn15 + sizeof(this->availableActionIn15) / sizeof(int));
            break;
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
            std::cout << "No available actions for final configurations." << std::endl;
            break;
        default:
            std::cout << "Invalid code for currConfiguration." << std::endl;
            break;
    }

    return availableActions;
}