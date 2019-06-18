//
// Created by Marco Matarese on 14/05/2019.
//

#ifndef BLOCKWORLDRL_BLOCKWORLDREINFORCEMENTLEARNER_H
#define BLOCKWORLDRL_BLOCKWORLDREINFORCEMENTLEARNER_H

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include "BlockWorldEnvironment.h"


class BlockWorldReinforcementLearner {

private:


    /* ---------------------------------------------------------------- */
    /* ---------------------------PARAMETERS--------------------------- */
    /* ---------------------------------------------------------------- */


    int QMatrixNoRows,
        QMatrixNoCols,
        *policy = nullptr;                  // policy[i] = best action's code in state i

    float   **QMatrix = nullptr,
            **rewardFunction = nullptr;

    BlockWorldEnvironment *perceivedEnv;


public:


    static const int    FINAL_ACTION_CODE = 11,
                        PERCENTAGE_OF_VICTORY = 75;


    /* ---------------------------------------------------------------- */
    /* --------------------------CONSTRUCTORS-------------------------- */
    /* ---------------------------------------------------------------- */


    /**
     * Constructor.
     * @param noRows
     * @param noCols
     * @param env
     */
    BlockWorldReinforcementLearner(int noRows, int noCols, BlockWorldEnvironment env) {
        this->QMatrixNoRows = noRows;
        this->QMatrixNoCols = noCols;

        this->QMatrix = new float*[noRows];

        for(int i = 0; i < this->QMatrixNoRows; i++)
            this->QMatrix[i] = new float[noCols];

        this->policy = new int[noRows];

        this->perceivedEnv = &env;

        //initializeRewardFunction();       // la implemento come un metodo
        //this->initializeQMatrix();
    }

    ~BlockWorldReinforcementLearner() {
        // TODO dai...
    }


    /* ---------------------------------------------------------------- */
    /* -------------------------GETTER-SETTER-------------------------- */
    /* ---------------------------------------------------------------- */


    /**
     * QMatrix getter.
     * @return this->QMatrix
     */
    inline float** getQMatrix() { return this->QMatrix; }

    /**
     * policy getter.
     * @return this->policy
     */
    inline int* getPolicy() { return this->policy; }

    /**
     * Getter.
     * @param indx
     * @return this->policy[indx] if exist; -1 otherwise
     */
    inline int getPolicyValue(int indx) {
        if(indx >= 0 && indx <= this->perceivedEnv->NO_CONFIGURATIONS - 1) {
            return this->policy[indx];
        }
        else {
            std::cout << "Invalid argument passed in getPolicyValue: " << indx << std::endl;
            return -1;
        }
    }

    /**
     * Getter.
     * @return this->perceivedEnv
     */
    inline BlockWorldEnvironment *getPerceivedEnv() { return this->perceivedEnv; }


    /* ---------------------------------------------------------------- */
    /* ----------------------------METHODS----------------------------- */
    /* ---------------------------------------------------------------- */


    /**
     * NON MI PIACE! LA IMPLEMENTO COME UN METODO
     */
    void initializeRewardFunction();

    /**
     * Initialize QMatrix.
     * The current initial value is 0.
     */
    void initializeQMatrix();

    /**
     * Print the QMatrix on std output.
     */
    void printQMatrix();

    /**
     *
     * @param noEpochs
     */
    void TemporalDifferenceRL(int noEpochs, int optimalPolicy[22]);

    /**
     *
     * @return
     */
    float calculateMeanSquareError();

    /**
     *
     * @param availableActions
     * @param e
     * @return
     */
    int chooseActionFromAvailableActions(std::vector<int> availableActions, float e);

    /**
     * Perceive the 'actionCode' action, this method changes the environment accordingly with 'transitionFunction'.
     * @param actionCode the action's code to do
     */
    void doAction(int actionCode);

    /**
     * Try to perceive the 'actionCode' action, this method changes the environment.
     * There is a high probability to act accordingly with 'transitionFunction' and a low probability to act
     * accordingly to 'unlikelyTransitionFunction'.
     * @param actionCode the action's code to do
     */
    void doStochasticAction(int actionCode);

    /**
     * Try to perceive the 'actionCode' action, this method changes the environment.
     * There is a high probability to act accordingly with 'transitionFunction' and a low probability to act random.
     * @param actionCode the action's code to do
     */
    void doStochasticRandomAction(int actionCode);

    /**
     * Calculate the reward function based on the agent's current state and the action already done.
     * Currently it is defined by cases.
     * @param state agent's current state
     * @param action the action already done
     * @return r(state, action)
     */
    float calculateReward(int state, int action);

    /**
     * Compute the policy based on this->QMatrix values.
     */
    void calculatePolicy();

    /**
     * Show the current policy.
     */
    void showPolicy();

    /**
     * Compute, for each state, the number of actions to arrive in the accepting configuration.
     * It uses BlockWorldEnvironment::ACCEPTING_CONFIGURATION_CODE!!!
     * @return an array of integer such that ret[i] = no actions to do in state i
     */
    int *getNoActionsToWin();
};

#endif //BLOCKWORLDRL_BLOCKWORLDREINFORCEMENTLEARNER_H
