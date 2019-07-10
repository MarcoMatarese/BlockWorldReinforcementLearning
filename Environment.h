//
// Created by Marco Matarese on 18/06/2019.
//

#ifndef ROBOT_GOVERNANCE_PROJECT_ENVIRONMENT_H
#define ROBOT_GOVERNANCE_PROJECT_ENVIRONMENT_H


#include "Predicate.h"
#include "Action.h"
#include <set>


class Environment {

private:

    int noStates,
        noBlocks,
        acceptingStateCode;

    std::set<Predicate> currState;


public:

    Environment(int noBls) {
        this->currState = std::set<Predicate>();// empty set of predicate
        this->noStates = 1;                     // at the beginning we know only one world configuration
        this->noBlocks = noBls;
        this->acceptingStateCode = -1;          // we set it when the final state will be encountered
        this->initCurrState();
    }

    ~Environment() {
        this->currState.clear();
    }


    /* ---------------------------------------------------------------- */
    /* -------------------------GETTER-SETTER-------------------------- */
    /* ---------------------------------------------------------------- */


    inline int getNoStates() {
        return this->noStates;
    }

    inline void setNoStates(int n) {
        this->noStates = n;
    }

    inline int getAcceptingStateCode() {
        return this->acceptingStateCode;
    }

    inline void setAcceptingStateCode(int code) {
        this->acceptingStateCode = code;
    }

    inline std::set<Predicate> getCurrState() {
        return this->currState;
    }

    inline int getNoBlocks() {
        return this->noBlocks;
    }


    /* ---------------------------------------------------------------- */
    /* ----------------------------METHODS----------------------------- */
    /* ---------------------------------------------------------------- */


    inline void addPredicateToCurrState(Predicate p) {
        std::set<Predicate>::iterator it = this->currState.begin();

        it = this->currState.insert(it, p);
    }

    inline void delPredicateToCurrState(Predicate p) {
        this->currState.erase(p);
    }

    /**
     * Initialize this->currState to the initial configuration: all blocks on the table and the hand empty.
     */
    void initCurrState();

    /**
     * Check if this->currState is a final configuration. The check is based on the number of predicates in
     * this->currState and their type.
     * @return false if there are a wrong number of predicates or there is much more than one block clear or
     * there is much more than one block on table; true otherwise.
     */
    bool isCurrStateAFinalState();

    /**
     *
     * @return
     */
    bool isCurrStateTheAcceptingState();

    /**
     *
     * @param pred
     */
    void addToCurrState(Predicate pred);

    /**
     *
     * @param pred
     */
    void removeToCurrState(Predicate pred);

    /**
     * Search in this->currState 'predName', but I have only partial info.
     * @param predName predicate's name
     * @param argIndx predName.args[argIndx] = arg
     * @param arg arguments I have
     * @return (the first occurrence of) the predicate I'm searching for, an empty predicate if it do not exists
     */
    Predicate findPredWithPartialInfoInCurrState(std::string predName, int argIndx, int arg);

    /**
     *
     * @param action
     * @return
     */
    bool currStateSatisfyPrecondsOf(Action action);
};


#endif //ROBOT_GOVERNANCE_PROJECT_ENVIRONMENT_H
