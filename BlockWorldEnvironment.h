//
// Created by Marco Matarese on 15/05/2019.
//

#ifndef BLOCKWORLDRL_BLOCKWORLDENVIRONMENT_H
#define BLOCKWORLDRL_BLOCKWORLDENVIRONMENT_H


#include <vector>
#include <iostream>
#include <random>


/**
 *
 * WORLD CONFIGURATIONS - RELATIVE CODES
 *
 * ABC  0
 *
 *  B                C               A               C               A               B
 * AC       1       AB      2       BC      3       BA      4       CB      5       CA  6
 *
 * [A]              [B]             [C]
 *    BC    7          AC   8          AB   9
 *
 * [A]B             [A]C            [B]A            [B]C            [C]A            [C]B
 *    C     10         B    11         C    12         A    13         B    14         A    15
 *
 * C                A               B               B               C               A
 * B                C               A               C               A               B
 * A        16      B       17      C       18      A       19      B       20      C       21
 *
 *
 * POSSIBLE ACTIONS - RELATIVE CODES
 *
 * takeA        0   takeB       1   takeC       2
 * putAonB      3   putAonC     4   putBonA     5
 * putBonC      6   putConA     7   putConB     8
 * putAonTable  9   putBonTable 10  putConTable 11
 */


class BlockWorldEnvironment {

private:


    /* ---------------------------------------------------------------- */
    /* ---------------------------PARAMETERS--------------------------- */
    /* ---------------------------------------------------------------- */


    int currConfiguration,

        availableActionIn0[3] = {0, 1, 2},
        availableActionIn1[2] = {0, 1},
        availableActionIn2[2] = {0, 2},
        availableActionIn3[2] = {0, 1},
        availableActionIn4[2] = {1, 2},
        availableActionIn5[2] = {0, 2},
        availableActionIn6[2] = {1, 2},
        availableActionIn7[3] = {3, 4, 9},
        availableActionIn8[3] = {5, 6, 10},
        availableActionIn9[3] = {7, 8, 11},
        availableActionIn10[2] = {3, 9},
        availableActionIn11[2] = {4, 9},
        availableActionIn12[2] = {5, 10},
        availableActionIn13[2] = {6, 10},
        availableActionIn14[2] = {7, 11},
        availableActionIn15[2] = {8, 11},

        **transitionFunction = nullptr;

    // for final states we need no actions
    /*    availableActionIn16[] = {},
        availableActionIn17[] = {},
        availableActionIn18[] = {},
        availableActionIn19[] = {},
        availableActionIn20[] = {},
        availableActionIn21[] = {}; */


public:


    /* ---------------------------------------------------------------- */
    /* ---------------------------PARAMETERS--------------------------- */
    /* ---------------------------------------------------------------- */


    static const int NO_CONFIGURATIONS = 22;
    static const int NO_ACTIONS = 12;
    static const int INITIAL_CONFIGURATION_CODE = 0;
    static const int ACCEPTING_CONFIGURATION_CODE = 21;
    static const int TERMINAL_CODES_RANGE_LEFT = 16;
    static const int TERMINAL_CODES_RANGE_RIGHT = 21;


    /* ---------------------------------------------------------------- */
    /* --------------------------CONSTRUCTORS-------------------------- */
    /* ---------------------------------------------------------------- */


    BlockWorldEnvironment() {

        this->currConfiguration = INITIAL_CONFIGURATION_CODE;

        this->transitionFunction = new int*[NO_CONFIGURATIONS];

        for(int i = 0; i < NO_CONFIGURATIONS; i++)
            this->transitionFunction[i] = new int[NO_ACTIONS];

        initTransitionFunction();

        //std::cout << availableActionIn0[0] << " " << availableActionIn0[1] << " " << availableActionIn0[2] << std::endl;
    }

    BlockWorldEnvironment(int conf) {
        if(conf < INITIAL_CONFIGURATION_CODE || conf > ACCEPTING_CONFIGURATION_CODE) {
            std::cout << "Invalid initial configuration code (" << conf << "). ";
            std::cout << "It must be between 0 and 21. ";
            std::cout << "A BlockWorldEnvironment with initial configuration will be created." << std::endl;

            BlockWorldEnvironment();
        }
        this->currConfiguration = conf;
    }


    /* ---------------------------------------------------------------- */
    /* -------------------------GETTER-SETTER-------------------------- */
    /* ---------------------------------------------------------------- */


    /**
     * Getter.
     * @return this->transitionFunction
     */
    inline int** getTransitionFunction() {
        return this->transitionFunction;
    }

    /**
     * Setter.
     * @param config next currConfig value
     */
    inline void setCurrConfiguration(int config) {
        this->currConfiguration = config;
    }

    inline int getARandomConfiguration() {
        return rand() % NO_CONFIGURATIONS;
    }

    /**
     * Getter.
     * @return this->currConfiguration
     */
    inline int getCurrConfiguration() {
        return this->currConfiguration;
    }


    /* ---------------------------------------------------------------- */
    /* ----------------------------METHODS----------------------------- */
    /* ---------------------------------------------------------------- */


    /**
     *
     * @param worldConfiguration
     * @return a vector containing the available actions in the current world configuration
     */
    std::vector<int> getAvailableActionsFromCurrentState();

    /**
     *
     * @param state
     * @return
     */
    std::vector<int> getAvailableActionsFromState(int state);

    /**
     * Initialize the transition matrix.
     */
    void initTransitionFunction();

    /**
     * Check whether the current world's configuration is a terminal one or not.
     * @param config the world's current configuration
     * @return true if config is a terminal state, false otherwise
     */
    inline bool isInTerminalConfiguration() {
        return  this->currConfiguration >= TERMINAL_CODES_RANGE_LEFT &&
                this->currConfiguration <= TERMINAL_CODES_RANGE_RIGHT;
    }
};


#endif //BLOCKWORLDRL_BLOCKWORLDENVIRONMENT_H
