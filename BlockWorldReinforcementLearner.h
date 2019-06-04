//
// Created by matar on 14/05/2019.
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
        QMatrixNoCols;

    //std::vector<std::vector<float>> QMatrix;
    float   **QMatrix = nullptr,
            **rewardFunction = nullptr;

    BlockWorldEnvironment *perceivedEnv;


public:


    static const int FINAL_ACTION_CODE = 11;


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

        this->perceivedEnv = &env;

        //initializeRewardFunction();       // la implemento come un metodo
        //this->initializeQMatrix();
    }

    ~BlockWorldReinforcementLearner() {

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
    void TemporalDifferenceRL(int noEpochs);

    /**
     *
     * @param availableActions
     * @param e
     * @return
     */
    int chooseActionFromAvailableActions(std::vector<int> availableActions, float e);

    /**
     *
     * @param actionCode
     */
    void doAction(int actionCode);

    /**
     * Calculate the reward function based on the agent's current state and the action already done.
     * Currently it is defined by cases.
     * @param state agent's current state
     * @param action the action already done
     * @return r(state, action)
     */
    float calculateReward(int state, int action);

    /**
     * Show the current policy.
     */
    void showPolicy();
};

#endif //BLOCKWORLDRL_BLOCKWORLDREINFORCEMENTLEARNER_H
