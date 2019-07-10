//
// Created by Marco Matarese on 18/06/2019.
//


#include "Agent.h"
#include <limits>
#include <list>
#include <time.h>


/**
 * Initialize this->codeToAction:
 * - all take(n) actions with n=0, ..., noBlocks-1
 * - all stack(n,m) actions with n,m=0, ..., noBlocks-1
 * - all putOnTable(n) actions with n=0, ..., noBlocks-1.
 */
void Agent::initCodeToAction() {

    int indx = 0,
        *args = new int[2];

    for(int i = 0; i < this->perceivedEnv->getNoBlocks(); i++) {
        args[0] = i;
        this->codeToAction.insert(std::pair<int, Action>(indx, Action("take", args)));
        this->actionToCode.insert(std::pair<Action, int>(Action("take", args), indx));
        indx++;
    }

    for(int i = 0; i < this->perceivedEnv->getNoBlocks(); i++) {
        args[0] = i;
        for(int j = 0; j < this->perceivedEnv->getNoBlocks(); j++) {
            if(i != j) {
                args[1] = j;
                this->codeToAction.insert(std::pair<int, Action>(indx, Action("stack", args)));
                this->actionToCode.insert(std::pair<Action, int>(Action("stack", args), indx));
                indx++;
            }
        }
    }

    for(int i = 0; i < this->perceivedEnv->getNoBlocks(); i++) {
        args[0] = i;
        this->codeToAction.insert(std::pair<int, Action>(indx, Action("putOnTable", args)));
        this->actionToCode.insert(std::pair<Action, int>(Action("putOnTable", args), indx));
        indx++;
    }
}

/**
 *
 */
void Agent::initCodeToStates() {
    this->codeToStates.insert(std::pair<int, std::set<Predicate>>(0, this->perceivedEnv->getCurrState()));
    this->statesToCode.insert(std::pair<std::set<Predicate>, int>(this->perceivedEnv->getCurrState(), 0));
}

/**
 * Initialize this->QMatrix.
 */
void Agent::initializeQMatrix() {

    int nextIndx;

    for(int i = 0; i < this->QMatrixNoRows; i++)
        for(int j = 0; j < this->QMatrixNoCols; j++)
            //this->QMatrix[i][j] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            this->QMatrix[i][j] = 0;
}

/**
 * Realloc this->QMatrix adding a new row.
 * Update this->QMatrixNoRows.
 * Update this->perceivedEnv->noStates.
 */
void Agent::addRowToQMatrix() {
    int currNoRows = this->QMatrixNoRows;

    // reallocation
    this->QMatrix = (float**) realloc(this->QMatrix, (currNoRows + 1) * sizeof(float *));
    this->QMatrix[currNoRows] = (float *) malloc(this->noActions * sizeof(float));
    for(int i = 0; i < this->noActions; i++)
        //this->QMatrix[currNoRows][i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        this->QMatrix[currNoRows][i] = 0;

    // updates
    this->QMatrixNoRows++;
    this->perceivedEnv->setNoStates(this->QMatrixNoRows);
}

/**
 * Print the QMatrix on std output.
 */
void Agent::printQMatrix() {
    for(int i = 0; i < this->QMatrixNoRows; i++) {
        for (int j = 0; j < this->QMatrixNoCols; j++)
            std::cout << this->QMatrix[i][j] << " ";
        std::cout << std::endl;
    }
}

/**
 * Compute the policy based on this->QMatrix values.
 */
void Agent::calculatePolicy() {

    int noConfigs = this->perceivedEnv->getNoStates(),
        noActions = this->noActions,
        currMaxIndx = 0;
    float currMax;
    std::set<Predicate> currConf;
    Action  currAction,
            bestActionPerConf;

    for(int i = 0; i < noConfigs; i++) {
        currMax = -9999999;
        currConf = this->codeToStates.at(i);
        for(int j = 0; j < noActions; j++) {
            currAction = this->codeToAction.at(j);
            if (this->QMatrix[i][j] != 0.0 && this->QMatrix[i][j] > currMax) {
                currMax = this->QMatrix[i][j];
                currMaxIndx = j;
            }
        }
        bestActionPerConf = this->codeToAction.at(currMaxIndx);
        this->policy.insert(std::pair<std::set<Predicate>, Action>(currConf, bestActionPerConf));
    }
}

/**
 * Show the current policy.
 */
void Agent::showPolicy() {

    std::cout << "BEST POLICY:" << std::endl;
    for(auto x : this->policy) {
        std::cout << "In state: ";
        for(auto y : x.first)
            std::cout << y << " ";
        std::cout << std::endl << " Do: " << x.second << std::endl;
    }
}

/**
 * TODO something to change!
 * Compute, for each state, the number of actions to arrive in the accepting configuration.
 * @return an array of integer such that ret[i] = no actions to do in state i to arrive in accepting configuration
 */
int *Agent::getNoActionsToWin() {

    int acceptConfig = this->perceivedEnv->getAcceptingStateCode(),
            noStates = this->perceivedEnv->getNoStates(),
            noActions = this->noActions,
            currConfig,
            iter,
            action,
            noActionsNeeded,
            *ret = new int[noStates];

    // TODO cambia il 16
    for(int i = 0; i < 16; i++) {
        noActionsNeeded = 0;
        currConfig = i;
        iter = 0;

        while(currConfig != acceptConfig && iter < noStates + noActions) {
            //action = this->getPolicyValue(currConfig);
            // TODO
            //currConfig = this->perceivedEnv->getTransitionFunction()[currConfig][action];
            noActionsNeeded++;
            iter++;
        }

        //if(currConfig != acceptConfig) ret[i] = max;
        ret[i] = noActionsNeeded;
    }

    return ret;
}

/**
 * Choose an action from all possible action in current state. Whit probability epsilon, choose the best action
 * based on Q value, with probability 1-epsilon another action.
 * @param epsilon the probability to do the best action
 * @return the chosen action
 */
Action Agent::chooseAction(float epsilon) {

    std::list<Action> possibleActions = std::list<Action>();
    std::list<Action>::iterator it = possibleActions.begin();

    Action  action,
            bestAction;

    bool precondsSatisfied;
    float currMax = -999999;
    int rowIndx = this->statesToCode[this->perceivedEnv->getCurrState()],
        colIndx = 0,
        prob = rand() % 100 + 1,
        indx = 0;

    //std::cout << "IN CHOOSE ACTION" << std::endl;
    //std::cout << "prob: " << prob << " " << epsilon * 100 << std::endl;
    //std::cout << "rowIndx: " << rowIndx << std::endl;

    // collect possible actions in current state
    for(int i = 0; i < noActions; i++) {
        action = this->codeToAction.at(i);
        /*std::cout << "prima di currStateSatisfy... "<< action << std::endl;
        for(auto p : *action.getPreconditions()) {
            std::cout << p << "" << std::endl;
        }*/
        precondsSatisfied = this->perceivedEnv->currStateSatisfyPrecondsOf(action);
        //std::cout << "action: "<< action << " indice mappa: " << this->actionToCode[action] << std::endl;
        //std::cout << "action: "<< action << " indice mappa: " << this->getActionToCode(action) << std::endl;
        //std::cout << "currStateSatisfy: " << std::boolalpha << precondsSatisfied << std::endl;
        if(precondsSatisfied) {
            //std::cout << "dentro precondsSatisfied" << std::endl;
            it = possibleActions.insert(it, action);
            colIndx = this->getActionToCode(action);
            if (colIndx == -1)
                std::cout << "ERROR: code to action " << action << " not found!" << std::endl;
            else {
                //std::cout << "colIndx: " << colIndx << " action: " << action << std::endl;
                if (this->QMatrix[rowIndx][colIndx] > currMax) {
                    currMax = this->QMatrix[rowIndx][colIndx];
                    bestAction = this->codeToAction.at(colIndx);
                    //std::cout << "best action: "<< bestAction << std::endl;
                }
            }
        }
    }

    if(prob < (epsilon * 100) && possibleActions.size() > 0) {                        // if prob < epsilon, then do another action
        indx = rand() % possibleActions.size();
        //std::cout << "indx: " << indx << std::endl;
        std::advance(it, indx);
        //std::cout << "*it: " << *it << std::endl;
        return *it;
        for(int i = 0; i < indx; i++)                   // I can't access directly to list elements... uff
            possibleActions.pop_front();
        if(possibleActions.size() > 0)                  // if there are other actions to do
            return possibleActions.front();
    }

    return bestAction;                                 // prob >= epsilon
}

/**
 *
 * @param currState
 * @param action
 * @return
 */
int Agent::calculateReward(Action action) {

    if(this->perceivedEnv->isCurrStateAFinalState()) {
        /*std::cout << "A FINAL STATE:" << std::endl;
        for(auto p : this->perceivedEnv->getCurrState())
            std::cout << p << " ";
        std::cout << std::endl;*/

        if(this->perceivedEnv->isCurrStateTheAcceptingState()) {
            /*std::cout << "ACCEPTING STATE:" << std::endl;
            for(auto p : this->perceivedEnv->getCurrState())
                std::cout << p << " ";
            std::cout << std::endl;*/
            return ACCEPTING_STATE_REWARD;
        }
        else return REJECTING_STATE_REWARD;
    }

    return ONE_STEP_REWARD;
}

/**
 * (1) Check if preconditions are satisfied.
 * (2) If satisfied, perform the action's effects (predicates to add and del).
 * @param action the action to do
 * @return true if action is performed, false otherwise
 */
bool Agent::doAction(Action action) {

    std::set<Predicate>  currState = this->perceivedEnv->getCurrState();

    std::set<Predicate>::iterator it;
    Predicate pred;
    int *args = new int[2];
    bool preconditionsSatisfied = this->perceivedEnv->currStateSatisfyPrecondsOf(action);

    if (preconditionsSatisfied) {
        // delete predicates
        for (Predicate pred : *(action.getPostconditionsToDel())) {
            //std::cout << "to del " << pred << std::endl;
            //it = this->perceivedEnv->getCurrState().find(pred);
            this->perceivedEnv->removeToCurrState(pred);
        }

        // add predicates
        for (Predicate pred : *(action.getPostconditionsToAdd())) {
            //std::cout << "to add " << pred << std::endl;
            //it = this->perceivedEnv->getCurrState().insert(it, pred);
            this->perceivedEnv->addToCurrState(pred);
            //std::cout << "effettivamente aggiunto " << *it << std::endl;
        }

        // non-automatic action effects: they depends on world's current configuration...
        if(action.getName() == "take") {
            args[0] = action.getArg(0);

            // if handling block was on another block
            if(! (pred = this->perceivedEnv->findPredWithPartialInfoInCurrState("on", 0, args[0])).isEmptyPredicate()) {
                args[1] = pred.getSecondArg();                                      // the other block
                this->perceivedEnv->removeToCurrState(Predicate("on", args));       // remove on(handling_b,other_b)
                args[0] = pred.getSecondArg();                                      //
                this->perceivedEnv->addToCurrState(Predicate("clear", args));       // remove clear(other_b)
            }
            // else if handling block was on table
            else if((it = this->perceivedEnv->getCurrState().find(Predicate("onTable", args))) !=
            this->perceivedEnv->getCurrState().end()) {
                this->perceivedEnv->removeToCurrState(Predicate("onTable", args));  // remove onTable(handling_b)
            }
        }
    }
    else {
        std::cout << "Precondition to action " << action.getName();
        for(int i = 0; i < action.getNoArgs(); i++)
            std::cout << " " << action.getArg(i) << " ";
        std::cout << "unsatisfied." << std::endl;
        return false;
    }

    return true;
    /*
    std::cout << "IN DO ACTION" << std::endl;
    for(Predicate p : this->getPerceivedEnv()->getCurrState()) {
        std::cout <<  p << std::endl;
    }
    std::cout << "--------------------" << std::endl;
    */
}

/**
 * Q learning with a non-well-know environment.
 * @param noEpochs
 */
void Agent::doQLearning(int noEpochs) {

    Action action;

    int actionCode = 0,
        currStateCode = 0,
        prevStateCode = 0,
        run,
        noVictories = 0;

    float   epsilon = 0.6,
            reward = 0,
            max_a = 0,
            alfa = 0.8,
            gamma = 0.9;

    std::set<Predicate> prevState,
                        currState;
    std::map<std::set<Predicate>, int>::iterator it;

    // for each epochs
    for(int i = 0; i < noEpochs; i++) {

        this->perceivedEnv->initCurrState();                            // init world's config
        run = 0;
        //std::cout << "ACTION TO CODE dopo initCurrState" << std::endl;
        /*for(auto a : this->getActionToCode())
            std::cout << a.first << " - " << a.second << std::endl;
        std::cout << "finito di stampare mappa" << std::endl;*/

        // until a final state is reached
        while(! this->perceivedEnv->isCurrStateAFinalState()) {

            action = this->chooseAction(epsilon);                       // choose an action

            /*std::cout << "ACTION TO CODE dopo chooseAction" << std::endl;
            for(auto a : this->getActionToCode())
                std::cout << a.first << " - " << a.second << std::endl;*/

            //std::cout << "IN DOQLEARNING" << std::endl;
            //std::cout << "action chosen: " << action << std::endl;
            prevState = this->perceivedEnv->getCurrState();             // collect actual state
                if(this->doAction(action)) {                                     // do the chosen action

                    /*std::cout << "ACTION TO CODE dopo doAction" << std::endl;
                    for(auto a : this->getActionToCode())
                        std::cout << a.first << " - " << a.second << std::endl;*/

                    currState = this->perceivedEnv->getCurrState();             // collect resulting state
                    reward = calculateReward(action);                           // calculate reward

                    if(reward == ACCEPTING_STATE_REWARD) noVictories++;

                    actionCode = this->getActionToCode(action);                    // retrieve Q col index

                    prevStateCode = this->statesToCode[prevState];              // retrieve Q row index (prev)

                    // check whether I already visited currState or not
                    if (this->statesToCode.find(currState) != this->statesToCode.end())
                        currStateCode = this->statesToCode[currState];          // retrieve Q row index (curr)
                    else {
                        this->codeToStates.insert(std::pair<int, std::set<Predicate>>(this->QMatrixNoRows, currState));
                        this->statesToCode.insert(std::pair<std::set<Predicate>, int>(currState, this->QMatrixNoRows));
                        currStateCode = this->QMatrixNoRows;
                        this->addRowToQMatrix();                                // add row to Q, update system's params
                    }

                    // retrieve best action in resulting state based on Q values
                    max_a = -1;
                    for (int i = 0; i < this->noActions; i++)
                        if (this->QMatrix[currStateCode][i] > max_a)
                            max_a = this->QMatrix[currStateCode][i];

                    // Q(s,a) = Q(s,a) + alfa * (r + gamma * max_a'(Q(s',a) - Q(s,a))
                    this->QMatrix[prevStateCode][actionCode] = this->QMatrix[prevStateCode][actionCode] +
                                                               alfa * (reward + gamma * max_a -
                                                                       this->QMatrix[prevStateCode][actionCode]);
                }
            std::cout << "Run " << run << ", Epoch " << i << std::endl;
            run++;
        }
    }

    std::cout << "Percentage of vinctory: " << (noVictories*100)/noEpochs << std::endl;
    std::cout << "Tot epochs: " << noEpochs << " victory runs: " << noVictories << std::endl;
}
