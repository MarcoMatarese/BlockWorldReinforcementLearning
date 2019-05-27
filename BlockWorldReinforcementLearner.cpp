//
// Created by matar on 14/05/2019.
//


#include "BlockWorldReinforcementLearner.h"


/**
 * NON MI PIACE! LA IMPLEMENTO COME UN METODO
 */
void BlockWorldReinforcementLearner::initializeRewardFunction() {
    int noConfigs = this->perceivedEnv->NO_CONFIGURATIONS,
        noActions = this->perceivedEnv->NO_ACTIONS;

    this->rewardFunction[0][0] = 0;                             // initial state has no reward

    for(int i = 0; i < noConfigs; i++)
        for(int j = 0; j < noActions; j++)
            this->rewardFunction[i][j] = -0.1;                  // all moves cost -0.1

    for(int i = 0; i < noActions; i++)
        this->rewardFunction[noConfigs - 1][i] = -1;            // terminal state have reward -1  (except the acceptation state)

    this->rewardFunction[noConfigs - 1][noActions - 1] = 1;     // acceptation state has reward 1
}

/**
 * Initialize this->QMatrix.
 */
void BlockWorldReinforcementLearner::initializeQMatrix() {
    std::vector<int> availableAct = std::vector<int>();
    int nextIndx;

    for(int i = 0; i < this->QMatrixNoRows; i++)
        for(int j = 0; j < this->QMatrixNoCols; j++)
            this->QMatrix[i][j] = 0;

    for(int i = 0; i < this->QMatrixNoRows; i++) {
        if (i < this->getPerceivedEnv()->TERMINAL_CODES_RANGE_LEFT) {
            availableAct = this->getPerceivedEnv()->getAvailableActionsFromState(i);
            for (int j = 0; j < availableAct.size(); j++) {
                nextIndx = availableAct[j];
                this->QMatrix[i][nextIndx] = static_cast <float> (rand()) /
                                             static_cast <float> (RAND_MAX);    // fill with the desired initial value
            }
        }
    }

    for(int i = 0; i < this->QMatrixNoCols; i++)
        this->QMatrix[this->QMatrixNoRows - 1][i] = 0;
}

/**
 * Print the QMatrix on std output.
 */
void BlockWorldReinforcementLearner::printQMatrix() {
    for(int i = 0; i < this->QMatrixNoRows; i++) {
        for (int j = 0; j < this->QMatrixNoCols; j++)
            std::cout << this->QMatrix[i][j] << " ";

        std::cout << std::endl;
    }
}

/**
 *
 * @param noEpochs number of epochs that will be executed.
 */
void BlockWorldReinforcementLearner::TemporalDifferenceRL(int noEpochs) {

    std::vector<int> availableActions = std::vector<int>();

    int action = 0,
        currState = 0,
        prevState = 0,
        run = 0;

    float   reward = 0,
            alfa = 0.8,     // step-size parameter
            gamma = 0.95,   // discount factor
            max_a = -1;

    // initialize QMatrix[s][a] for all s, a and QMatrix[terminal_state][a]=0 for all a
    initializeQMatrix();

    // repeat for each episode...
    for(int currEpoch = 0; currEpoch < noEpochs; currEpoch++) {

        perceivedEnv->setCurrConfiguration(0);                               // initialize S

        run = 0;
        // repeat for each step of episode, until s is terminal...
        while(! perceivedEnv->isInTerminalConfiguration()) {
            availableActions = perceivedEnv->getAvailableActionsFromCurrentState();

            action = chooseActionFromAvailableActions(availableActions);    // choose a from available actions s using policy derived from QMatrix (e-greedy)

            prevState = this->perceivedEnv->getCurrConfiguration();
            doAction(action);                                               // do the chosen action and perceive the next state
            currState = this->perceivedEnv->getCurrConfiguration();          //

            reward = calculateReward(currState, action);                    // calculate the reward based on the current state and the executed action

            max_a = -1;
            for(int i = 0; i < BlockWorldEnvironment::NO_ACTIONS; i++)
                if(this->QMatrix[currState][i] > max_a)
                    max_a = this->QMatrix[currState][i];                    // calculate the max expected value on the next state

            // Q(s, a) = Q(s, a) + alfa * (r + gamma * max_a(Q(s', a) - Q(s, a))
            this->QMatrix[prevState][action] = QMatrix[prevState][action] + alfa * (reward + gamma * max_a - QMatrix[prevState][action]);

            run++;
            std::cout << "run " << run << " epoca " << currEpoch << " stato " << prevState << " scelto azione " << action << " stato risultante " << currState << std::endl;
            /*std::cout << "QMATRIX EPOCA " << currEpoch << std::endl;
            printQMatrix();
            std::cout << std::endl;*/
        }

        /*std::cout << "QMATRIX EPOCA " << currEpoch << std::endl;
        printQMatrix();
        std::cout << std::endl;*/
    }
}

/**
 * Choose one action from the available actions vector with e-greedy policy.
 * e-greedy policy chooses the action with the greater value.
 * @param availableActions available actions vector
 * @return the chosen action
 */
int BlockWorldReinforcementLearner::chooseActionFromAvailableActions(std::vector<int> availableActions) {

    int currState = this->perceivedEnv->getCurrConfiguration(),
        tmpAction = -1,
        incumbent = -1;

    for(int currAction = 0; currAction < this->QMatrixNoCols; currAction++) {
        if(this->QMatrix[currState][currAction] > incumbent) {
            incumbent = currAction;
            // check whether currAction is available or not
            /*for(int i = 0; i < availableActions.size(); i++)
                if(availableActions[i] == currAction){
                    incumbent = currAction;
                    break;
                }*/
        }
    }

    return incumbent;
}

/**
 * Change the current world's configuration in base of the selected action.
 *
 * @param actionCode the selected action's code
 */
void BlockWorldReinforcementLearner::doAction(int actionCode) {

    int currState = this->perceivedEnv->getCurrConfiguration(),                              // get current world's configuration
        resultingState = this->perceivedEnv->getTransitionFunction()[currState][actionCode]; // calculate next state with the transition function

    this->perceivedEnv->setCurrConfiguration(resultingState);                                // operate changes on the environment
}

/**
 * Calculate the reward function based on the agent's current state and the action already done.
 * Currently it is defined by cases.
 * @param state agent's current state
 * @param action the action already done
 * @return r(state, action)
 */
float BlockWorldReinforcementLearner::calculateReward(int state, int action) {
    float reward = 0;
    int noConfigs = this->perceivedEnv->NO_CONFIGURATIONS,
        noActions = this->perceivedEnv->NO_ACTIONS;

    // validation control
    if(state < 0 || state > BlockWorldEnvironment::NO_CONFIGURATIONS ||
    action < 0 || action > BlockWorldEnvironment::NO_ACTIONS)
        return 0;

    if(state == noConfigs - 1) {                    // if I'm going to a terminal state
        if(action == noActions - 1) reward = 1;     // acceptation reward
        else reward = -1;                           // other terminal states' reward
    }
    else
        reward = -0.1;                              // any other state's reward

    return reward;
}

/**
 * Show the current policy.
 */
void BlockWorldReinforcementLearner::showPolicy() {

    int noConfigs = BlockWorldEnvironment::NO_CONFIGURATIONS,
        noActions = BlockWorldEnvironment::NO_ACTIONS,
        //policy[noConfigs],
        currMaxIndx = 0;

    float currMax;

    // retrieve and show best policy (e-greedy) from QMatrix
    for(int i = 0; i < noConfigs; i++) {
        currMax = -1;
        for(int j = 0; j < noActions; j++)
            if(this->QMatrix[i][j] > currMax) {
                currMax = this->QMatrix[i][j];
                currMaxIndx = j;
            }
        std::cout << "In state " << i << " do action " << currMaxIndx << std::endl;
        //policy[i] = currMaxIndx;
    }

}
