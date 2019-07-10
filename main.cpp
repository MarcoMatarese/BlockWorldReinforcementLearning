#include <iostream>
#include "Predicate.h"
#include "Action.h"
#include "Agent.h"

// TODO find a method to check whether it has learned or not (idea: percentage of vinctories)

#define NO_BLOCKS 5

int main() {
    std::cout << "Hello, Marco!" << std::endl;

    std::set<Predicate> preds1 = std::set<Predicate>(),
                        preds2 = std::set<Predicate>();
    int *args = new int[2];
    Environment env = Environment(NO_BLOCKS);
    Agent r = Agent(env, NO_BLOCKS);

    //r.getPerceivedEnv()->initCurrState();

    r.doQLearning(4000);

    //r.calculatePolicy();
    //r.showPolicy();

/*
    std::cout << "code to states size: " << r.getCodeToStates().size() << std::endl;
    std::cout << "CODE TO STATES" << std::endl;
    for(auto s : r.getCodeToStates()) {
        std::cout << s.first << ": ";
        for (auto p : s.second)
            std::cout << p << " ";
        std::cout << std::endl;
    }
    std::cout << "states to code size: " << r.getStatesToCode().size() << std::endl;
    std::cout << "STATES TO CODE" << std::endl;
    for(auto s : r.getStatesToCode()) {
        for (auto p : s.first)
            std::cout << p << " ";
        std::cout << std::endl <<  s.second << std::endl;
    }
*/

    /*
    std::cout << "noactions:" << r.getNoActions() << std::endl;
    std::cout << "QMATRIX:" << std::endl;
    for(int i = 0; i < r.getQMatrixNoRows(); i++) {
        for(int j = 0; j < r.getQMatrixNoCols(); j++) {
            std::cout << r.getQMatrixValue(i, j) << " ";
        }
        std::cout << std::endl;
    }
    r.addRowToQMatrix();
    std::cout << "QMATRIX:" << std::endl;
    for(int i = 0; i < r.getQMatrixNoRows(); i++) {
        for(int j = 0; j < r.getQMatrixNoCols(); j++) {
            std::cout << r.getQMatrixValue(i, j) << " ";
        }
        std::cout << std::endl;
    }
     */
    /*
    // test mappe
    std::cout << "CODE TO ACTION" << std::endl;
    for(auto a : r.getCodeToAction())
        std::cout << a.first << " - " << a.second << std::endl;
    std::cout << "ACTION TO CODE" << std::endl;
    for(auto a : r.getActionToCode())
        std::cout << a.first << " - " << a.second << std::endl;

    std::cout << "chosen action: " << r.chooseAction(0.7) << std::endl;
    */

    /*
    // test pre/post-condizioni
    for(auto p : *Action("putOnTable", args).getPostconditionsToDel())
        std::cout << p << "  ";
    std::cout << std::endl;
    */

    /*
    std::cout << "currState size: " << r.getPerceivedEnv()->getCurrState().size() << std::endl;
    std::cout << "noBlocks: " << r.getPerceivedEnv()->getNoBlocks() << std::endl;
    for(auto p : r.getPerceivedEnv()->getCurrState()) {
        std::cout <<  p << std::endl;
    }
    */

    return 0;
}