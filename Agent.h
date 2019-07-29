//
// Created by Marco Matarese on 18/06/2019.
//

#ifndef ROBOT_GOVERNANCE_PROJECT_AGENT_H
#define ROBOT_GOVERNANCE_PROJECT_AGENT_H


#include "Environment.h"
#include "Predicate.h"
#include "Action.h"
#include <cmath>
#include <map>
#include <string>


#define ACCEPTING_STATE_REWARD 10
#define REJECTING_STATE_REWARD -10
#define ONE_STEP_REWARD -1


class Agent {


private:

    int QMatrixNoRows,
        QMatrixNoCols,
        noActions;
        //*policy = nullptr;      // policy[i]=best action in state i

    float **QMatrix = nullptr;

    Environment *perceivedEnv;

    std::map<int, std::set<Predicate>> codeToStates;
    std::map<std::set<Predicate>, int> statesToCode;
    std::map<int, Action> codeToAction;
    std::map<Action, int> actionToCode;
    std::map<std::set<Predicate>, Action> policy;



public:

    /* ---------------------------------------------------------------- */
    /* --------------------------CONSTRUCTORS-------------------------- */
    /* ---------------------------------------------------------------- */


    Agent(Environment env, int noBlocks) {

        this->noActions = pow(noBlocks, 2) + noBlocks;

        this->QMatrixNoRows = 1;
        this->QMatrixNoCols = this->noActions;

        this->QMatrix = (float **) malloc(this->QMatrixNoRows * sizeof(float*));
        for(int i = 0; i < this->QMatrixNoRows; i++)
            this->QMatrix[i] = (float *) malloc(this->QMatrixNoCols * sizeof(float));

        this->perceivedEnv = &env;

        this->codeToStates = std::map<int, std::set<Predicate>>();  // Q Matrix row entry - world's config
        this->statesToCode = std::map<std::set<Predicate>, int>();  // reverse of codeToStates
        this->codeToAction = std::map<int, Action>();               // Q Matrix col entry - action
        this->actionToCode = std::map<Action, int>();               // reverse of codeToAction
        this->policy = std::map<std::set<Predicate>, Action>();

        this->initCodeToAction();
        this->initCodeToStates();
        this->initializeQMatrix();
    }

    ~Agent() {
        // TODO there are others things to DESTROY!!!
        for(int i = 0; i < this->QMatrixNoRows; i++)
            delete[] this->QMatrix[i];
        delete this->QMatrix;
    }


    /* ---------------------------------------------------------------- */
    /* -------------------------GETTER-SETTER-------------------------- */
    /* ---------------------------------------------------------------- */


    inline void setQMatrixNoRows(int noRows) {
        this->QMatrixNoRows = noRows;
    }

    inline int getQMatrixNoRows() {
        return this->QMatrixNoRows;
    }

    inline void setQMatrixNoCols(int noCols) {
        this->QMatrixNoCols = noCols;
    }

    inline int getQMatrixNoCols() {
        return this->QMatrixNoCols;
    }

    inline float getQMatrixValue(int i, int j) {
        return this->QMatrix[i][j];
    }

    inline Environment* getPerceivedEnv() {
        return this->perceivedEnv;
    }

    /**
     * Getter of this->codeToAction
     * @param code
     * @return the action associated to 'code' or this->codeToAction.end() if it not exists
     */
    int getActionToCode(Action act) {
        for(auto a : this->actionToCode)
            if(a.first == act) return a.second;
        return -1;
    }

    int getNoActions() {
        return this->noActions;
    }

    std::map<int, Action> getCodeToAction() {
        return this->codeToAction;
    }

    std::map<Action, int> getActionToCode() {
        return this->actionToCode;
    }

    std::map<int, std::set<Predicate>> getCodeToStates() {
        return this->codeToStates;
    }

    std::map<std::set<Predicate>, int> getStatesToCode() {
        return this->statesToCode;
    }


    /* ---------------------------------------------------------------- */
    /* ----------------------------METHODS----------------------------- */
    /* ---------------------------------------------------------------- */


    /**
     *
     */
    void initCodeToAction();

    /**
     *
     */
    void initCodeToStates();

    /**
     * Initialize QMatrix.
     * The current initial value is 0.
     */
    void initializeQMatrix();

    /**
     * Realloc this->QMatrix adding a new row.
     * Update this->QMatrixNoRows.
     * Update this->perceivedEnv->noStates.
     */
    void addRowToQMatrix();

    /**
     * Print the QMatrix on std output.
     */
    void printQMatrix();

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
     * @return an array of integer such that ret[i] = no actions to do in state i
     */
    int *getNoActionsToWin();

    /**
     *
     * @param epsilon
     * @return
     */
    Action chooseAction(float epsilon);

    /**
     *
     * @param currState
     * @param action
     * @return
     */
    int calculateReward(Action action);

    /**
     *
     * @param action the action to do
     * @return true if action is performed, false otherwise
     */
    bool doAction(Action action);

    /**
     *
     * @param noEpochs
     */
    void doQLearning(int noEpochs);
};


#endif //ROBOT_GOVERNANCE_PROJECT_AGENT_H
