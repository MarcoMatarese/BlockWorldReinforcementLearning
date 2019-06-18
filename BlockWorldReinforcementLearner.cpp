//
// Created by Marco Matarese on 14/05/2019.
//


#include <limits>
#include <random>
#include <ctime>
#include <cmath>
#include <math.h>

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
 * Q learning method. It uses Mean Square Error.
 * @param noEpochs number of max epochs.
 */
void BlockWorldReinforcementLearner::TemporalDifferenceRL(int noEpochs, int optimalPolicy[22]) {

    std::vector<int> availableActions = std::vector<int>();

    int action = 0,
        currState = 0,
        prevState = 0,
        initState,
        run = 0,
        noTotRuns = 0,
        noVinctory = 0,
        noStates = this->perceivedEnv->NO_CONFIGURATIONS,
        noActions = this->perceivedEnv->NO_ACTIONS,
        *frequencies = new int[22](),                        // frequencies of 'the right' choice per state
        *noRunsPerState = new int[22]();                    // for each state, what time we were been in that state

    float   reward = 0,
            epsilon = 0.4,  // epsilon used in e-greedy policy
            alfa = 0.8,     // step-size parameter
            gamma = 0.9,    // discount factor
            max_a = -1,
            percentage_victory = 0.0;

    double meanSquareError = 10.0,
            prevValueFunction,
            currValueFunction,
            errorThreshold = 0.005;

    // initialize QMatrix[s][a] for all s, a and QMatrix[terminal_state][a]=0 for all a
    initializeQMatrix();

    // repeat for each episode... (meanSquareError - errorThreshold >= 0 || currEpoch <= noEpochs/3) &&
    for(int currEpoch = 0; (meanSquareError - errorThreshold >= 0 || currEpoch <= noEpochs/3) && currEpoch < noEpochs; currEpoch++) {

        perceivedEnv->setCurrConfiguration(0);                // initialize S
        //initState = perceivedEnv->getARandomConfiguration();
        //perceivedEnv->setCurrConfiguration(initState);

        run = 0;
        meanSquareError = 0.0;
        prevValueFunction = 0.0;
        currValueFunction = 0.0;

        /*for(int i = 0; i < noStates; i++)
            for(int j = 0; j < noActions; j++)
                prevValueFunction += this->QMatrix[i][j];*/

        // repeat for each step of episode, until s is terminal...
        while(! perceivedEnv->isInTerminalConfiguration()) {

            availableActions = perceivedEnv->getAvailableActionsFromCurrentState();

            action = chooseActionFromAvailableActions(availableActions, epsilon);   // choose a from available actions s using policy derived from QMatrix (e-greedy)

            prevState = this->perceivedEnv->getCurrConfiguration();

            noRunsPerState[prevState]++;    // each time I go in prevState

            doAction(action);                                                       // do the chosen action and perceive the next state

            if(action == optimalPolicy[prevState]) frequencies[prevState]++; // if I choose the optimal action

            //doStochasticAction(action);                                             // try (in a stochastic way) to do the chosen action
            //doStochasticRandomAction(action);                                     // try (in a stochastic/random way) to do the chosen action
            currState = this->perceivedEnv->getCurrConfiguration();                 //

            reward = calculateReward(currState, action);                            // calculate the reward based on the current state and the executed action

            // calculate the max expected value on the next state
            max_a = -1;
            for(int i = 0; i < BlockWorldEnvironment::NO_ACTIONS; i++)
                if(this->QMatrix[currState][i] > max_a)
                    max_a = this->QMatrix[currState][i];

            // update mean square error (pt. 1)
            prevValueFunction = this->QMatrix[prevState][action];

            // Q(s, a) = Q(s, a) + alfa * (r + gamma * max_a(Q(s', a) - Q(s, a))
            this->QMatrix[prevState][action] = QMatrix[prevState][action] + alfa * (reward + gamma * max_a - QMatrix[prevState][action]);

            // update mean square error (pt. 2)
            currValueFunction = this->QMatrix[prevState][action];
            meanSquareError += abs(prevValueFunction - currValueFunction);    // after it divide these square errors by |S|

            run++;
            noTotRuns++;

            if(currState == this->perceivedEnv->ACCEPTING_CONFIGURATION_CODE) noVinctory++;

            percentage_victory = ((float)noVinctory / (float)currEpoch) * 100.0;
            //std::cout << "noVictory " << noVinctory << std::endl;
            //std::cout << "run " << run << " epoca " << currEpoch << " stato " << prevState << " scelto azione " << action << " stato risultante " << currState << std::endl;
            //std::cout << "QMATRIX" << std::endl;
            //printQMatrix();
            std::cout << "run " << run << " - epoca " << currEpoch << " - vittorie in % " << percentage_victory << std::endl;
        }

        /*for(int i = 0; i < noStates; i++)
            for(int j = 0; j < noActions; j++)
                currValueFunction += this->QMatrix[i][j];*/

        //meanSquareError = sqrt(abs(prevValueFunction - currValueFunction)) / noStates;
        meanSquareError = sqrt(meanSquareError) / noStates;
        std::cout << "Mean Square Error: " << meanSquareError << std::endl;
        std::cout << "Mean Square Error - threshold: " << meanSquareError  - errorThreshold << std::endl;
    }

    std::cout << "OPTIMAL Frequencies:" << std::endl;
    for(int i = 0; i < 16; i++)
        std::cout << "freq. " << i << " = " << (float)frequencies[i]/noRunsPerState[i] << std::endl;
}

/**
 *
 */
float BlockWorldReinforcementLearner::calculateMeanSquareError() {

    int noStates = this->perceivedEnv->NO_CONFIGURATIONS,
        noActions = this->perceivedEnv->NO_ACTIONS;
    float meanSquareError = 0.0;

    for(int i = 0; i < noStates; i++)
        for(int j = 0; j < noActions; j++)
            i = i;
            //meanSquareError += sqrt(vecchioQ[i][j] - nuovoQ[i][j])

    return meanSquareError;
}

/**
 * Choose one action from the available actions vector with e-greedy policy.
 * e-greedy policy chooses the action with the greater value of q.
 * @param availableActions available actions vector
 * @param e epsilon
 * @return the chosen action
 */
int BlockWorldReinforcementLearner::chooseActionFromAvailableActions(std::vector<int> availableActions, float e) {

    int currState = this->perceivedEnv->getCurrConfiguration(),
        action = -1,
        tmpAction,
        indx = 0,
        prob = rand() % 100 + 1;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    float   incumbent = -1 * (std::numeric_limits<float>::max());

    // get the action related to the max value of Q
    for (std::vector<int>::iterator it = availableActions.begin(); it != availableActions.end(); it++) {
        if (this->QMatrix[currState][*it] > incumbent) {
            incumbent = this->QMatrix[currState][*it];
            action = *it;
        }
    }

    if(prob < (e * 100)) {
        // exploration

        tmpAction = action;
        while(tmpAction == action) {
            //srand(time(NULL));
            indx = rand() % availableActions.size();
            action = availableActions[indx];
        }
    }
    // else exploitation

    return action;
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
 * Try to perceive the 'actionCode' action, this method changes the environment.
 * There is a high probability to act accordingly with 'transitionFunction' and a low probability to act
 * accordingly to 'unlikelyTransitionFunction'.
 * @param actionCode the action's code to do
 */
void BlockWorldReinforcementLearner::doStochasticAction(int actionCode) {

    int currState = this->perceivedEnv->getCurrConfiguration(),         // get current world's configuration
        prob = rand() % 100 + 1,
        resultingState = -1;

    if(prob <= this->perceivedEnv->PROBABILITY_TO_SUCCESS * 100)
        resultingState = this->perceivedEnv->getTransitionFunction()[currState][actionCode];
    else
        resultingState = this->perceivedEnv->getUnlikelyTransitionFunction()[currState][actionCode];

    this->perceivedEnv->setCurrConfiguration(resultingState);
}

/**
 * Try to perceive the 'actionCode' action, this method changes the environment.
 * There is a high probability to act accordingly with 'transitionFunction' and a low probability to act random.
 * @param actionCode the action's code to do
 */
void BlockWorldReinforcementLearner::doStochasticRandomAction(int actionCode) {

    int currState = this->perceivedEnv->getCurrConfiguration(),         // get current world's configuration
            prob = rand() % 100 + 1,
            resultingState = this->perceivedEnv->getTransitionFunction()[currState][actionCode];

    if(prob > this->perceivedEnv->PROBABILITY_TO_SUCCESS * 100)
        resultingState = this->perceivedEnv->getARandomConfigurationExcept(resultingState);

    this->perceivedEnv->setCurrConfiguration(resultingState);
}

/**
 * Calculate the reward function based on the agent's current state and the action already done.
 * Currently it is defined by cases.
 * @param state agent's current state
 * @param action the action already done
 * @return r(state, action)
 */
float BlockWorldReinforcementLearner::calculateReward(int state, int action) {

    float reward = -1;

    int noConfigs = this->perceivedEnv->NO_CONFIGURATIONS,
        noActions = this->perceivedEnv->NO_ACTIONS,
        acceptingState = this->perceivedEnv->ACCEPTING_CONFIGURATION_CODE;

    // validation control
    if(state < 0 || state >= noConfigs ||
    action < 0 || action >= noActions)
        return 0;

    if(state == acceptingState) reward = 10;            // acceptation reward
    else if(state >= 16 && state <= 20) reward = -10;   // other terminal states' reward
    else reward = -1;                                   // any other state's reward

    return reward;
}

/**
 * Compute the policy based on this->QMatrix values.
 */
void BlockWorldReinforcementLearner::calculatePolicy() {

    int noConfigs = BlockWorldEnvironment::NO_CONFIGURATIONS,
        noActions = BlockWorldEnvironment::NO_ACTIONS,
        currMaxIndx = 0;

    float currMax;

    for(int i = 0; i < noConfigs; i++) {
        currMax = -1;
        for(int j = 0; j < noActions; j++)
            if(this->QMatrix[i][j] != 0.0 && this->QMatrix[i][j] > currMax) {
                currMax = this->QMatrix[i][j];
                currMaxIndx = j;
            }
        this->policy[i] = currMaxIndx;
    }
}

/**
 * Show the current policy.
 */
void BlockWorldReinforcementLearner::showPolicy() {

    int noConfigs = BlockWorldEnvironment::NO_CONFIGURATIONS;

    for(int i = 0; i < noConfigs; i++)
        std::cout << "In state " << i << " do action " << this->getPolicyValue(i) << std::endl;
}

/**
 * Compute, for each state, the number of actions to arrive in the accepting configuration.
 * It uses BlockWorldEnvironment::ACCEPTING_CONFIGURATION_CODE and the transition function!!!
 * @return an array of integer such that ret[i] = no actions to do in state i to arrive in accepting configuration
 */
int *BlockWorldReinforcementLearner::getNoActionsToWin() {

    int acceptConfig = this->perceivedEnv->ACCEPTING_CONFIGURATION_CODE,
        noStates = this->perceivedEnv->NO_CONFIGURATIONS,
        noActions = this->perceivedEnv->NO_ACTIONS,
        currConfig,
        iter,
        action,
        noActionsNeeded,
        *ret = new int[noStates];

    int max = std::numeric_limits<int>::max();

    // TODO cambia il 16
    for(int i = 0; i < 16; i++) {
        noActionsNeeded = 0;
        currConfig = i;
        iter = 0;

        while(currConfig != acceptConfig && iter < noStates + noActions) {
            action = this->getPolicyValue(currConfig);
            currConfig = this->perceivedEnv->getTransitionFunction()[currConfig][action];
            noActionsNeeded++;
            iter++;
        }

        if(currConfig != acceptConfig) ret[i] = max;
        ret[i] = noActionsNeeded;
    }

    return ret;
}
