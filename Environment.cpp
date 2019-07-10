//
// Created by Marco Matarese on 18/06/2019.
//


#include "Environment.h"


/**
 * Initialize this->currState to the initial configuration: all blocks on the table and the hand empty.
 */
void Environment::initCurrState() {

    std::set<Predicate>::iterator it = this->currState.begin();
    int *args = new int[1];
    Predicate pred;

    if(! this->currState.empty()) {
        this->currState.clear();
    }

    for(int i = 0; i < this->noBlocks; i++) {
        args[0] = i;
        pred = Predicate("onTable", args);
        it = this->currState.insert(it, pred);
        pred = Predicate("clear", args);
        it = this->currState.insert(it, pred);
    }

    it = this->currState.insert(it, Predicate("handEmpty", args));
}

/**
 * Check if this->currState is a final configuration. The check is based on the number of predicates in
 * this->currState and their type.
 * @return false if there are a wrong number of predicates or there is much more than one block clear or
 * there is much more than one block on table; true otherwise.
 */
bool Environment::isCurrStateAFinalState() {

    // I can't use set.count because of the implementation of the operator ==
    int cnt = 0;

    // check the number of predicates: n-1 on; 1 onTable; 1 clear; 1 handEmpty
    if(this->currState.size() != this->noBlocks + 2) return false;

    // check if there is only one clear
    for(auto it : this->currState)
        if(it.getName() == "clear") cnt++;

    if(cnt != 1) return false;
    cnt = 0;

    // check if there is only one onTable
    for(auto it : this->currState)
        if(it.getName() == "onTable") cnt++;

    if(cnt != 1) return false;

    return true;
}

/**
 *
 * @return
 */
bool Environment::isCurrStateTheAcceptingState() {
    for(Predicate p : this->currState) {
        if(p.getName() == "on" && (p.getArg(0) - p.getArg(1)) != -1) {
            //std::cout << "primo if on " << p.getArg(0) << " " << p.getArg(1) << std::endl;
            return false;
        }
        else if(p.getName() == "clear" && p.getArg(0) != 0) {
            //std::cout << "secondo if clear " << p.getArg(0) << std::endl;
            return false;
        }
        else if(p.getName() == "onTable" && p.getArg(0) != this->noBlocks - 1) {
            //std::cout << "terzo if ontable " << p.getArg(0) << std::endl;
            return false;
        }
        else if(p.getName() == "handEmpty") {
            //std::cout << "quarto if handEmpty " << std::endl;
            continue;
        }
    }
    return true;
}

/**
 *
 * @param pred
 */
void Environment::addToCurrState(Predicate pred) {
    std::set<Predicate>::iterator it = this->currState.begin();
    it = this->currState.insert(it, pred);
    //std::cout << "(addToCurrState) pred: " << pred << " it: " << *it << std::endl;
    //this->currState.insert(pred);
}

/**
 *
 * @param pred
 */
void Environment::removeToCurrState(Predicate pred) {
    //std::cout << "(removeToCurrState) pred: " << pred << std::endl;
    this->currState.erase(pred);
}

/**
 * Search in this->currState 'predName', but I have only partial info.
 * ATTENTION: Currently only with noArgs=2 TODO
 * @param predName predicate's name
 * @param noArgs number of predicate arguments (noArgs > len(args))
 * @param args arguments I have
 * @return a ref to (the first occurrence of) the predicate I'm searching for, nullptr if it do not exists
 */
Predicate Environment::findPredWithPartialInfoInCurrState(std::string predName, int argIndx, int arg) {

    int *args = new int[2];
    args[0] = arg;

    for(Predicate pred : this->currState) {
        if(pred.getName() == predName and pred.getArg(argIndx) == arg) {
            args[1] = pred.getSecondArg();
            return *this->currState.find(Predicate(predName, args));
        }
    }

    return Predicate("emptyPredicate");
}

/**
 * Check whether currState satisfy preconditions on 'action' or not.
 * @param action
 * @return true if this->currState satisfy all action.getPreconditions(), false otherwise
 */
bool Environment::currStateSatisfyPrecondsOf(Action action) {

    std::set<Predicate>  currState = this->getCurrState();
    std::set<Predicate>::iterator it;

    // check preconditions
    for (Predicate precond : *action.getPreconditions()) {
        it = currState.find(precond);                       // find preconditions in current state
        if (it == currState.end()) return false;            // preconditions unsatisfied
    }

    return true;                                            // preconditions satisfied
}