//
// Created by Marco Matarese on 18/06/2019.
//

#ifndef ROBOT_GOVERNANCE_PROJECT_ACTION_H
#define ROBOT_GOVERNANCE_PROJECT_ACTION_H


#include <string>
#include <set>
#include "Predicate.h"


class Action {


private:


    std::string name;

    int noArgs,
        *args = nullptr;

    std::set<Predicate> *preconditions,
                        *postConditionsToAdd,
                        *postConditionsToDel;


public:


    /**
     *
     * @param name
     * @param args
     */
    Action(std::string name, int *args) {

        int *reverseArgs = new int[2];
        this->name = name;
        this->preconditions = new std::set<Predicate>();
        this->postConditionsToAdd = new std::set<Predicate>();
        this->postConditionsToDel = new std::set<Predicate>();

        std::set<Predicate>::iterator itPre = this->preconditions->begin();
        std::set<Predicate>::iterator itPostAdd = this->postConditionsToAdd->begin();
        std::set<Predicate>::iterator itPostDel = this->postConditionsToDel->begin();

        if(name == "take") {            // take(A)
            this->noArgs = 1;
            this->args = new int[noArgs];
            for(int i = 0, j = noArgs - 1; i < noArgs; i++, j--) {
                this->args[i] = args[i];
                reverseArgs[j] = args[i];
            }

            itPre = this->preconditions->insert(itPre, Predicate("handEmpty", args));               // handEmpty()
            itPre = this->preconditions->insert(itPre, Predicate("clear", args));                   // clear(A)

            itPostAdd = this->postConditionsToAdd->insert(itPostAdd, Predicate("holding", args));   // holding(A)

            // in doAction method, add clear(B) to post-conditions if on(A, B); del onTable(A) if onTable(A)

            itPostDel = this->postConditionsToDel->insert(itPostDel, Predicate("handEmpty", args)); // handEmpty()
            itPostDel = this->postConditionsToDel->insert(itPostDel, Predicate("clear", args));     // clear(A)
            itPostDel = this->postConditionsToDel->insert(itPostDel, Predicate("onTable", args));   // onTable(A), it may not exists
        }
        else if(name == "stack") {      // stack(A, B)
            this->noArgs = 2;
            this->args = new int[noArgs];
            for(int i = 0, j = noArgs - 1; i < noArgs; i++, j--) {
                this->args[i] = args[i];
                reverseArgs[j] = args[i];
            }

            itPre = this->preconditions->insert(itPre, Predicate("holding", args));                     // holding(A)
            itPre = this->preconditions->insert(itPre, Predicate("clear", reverseArgs));                // clear(B)

            itPostAdd = this->postConditionsToAdd->insert(itPostAdd, Predicate("on", args));            // on(A, B)
            itPostAdd = this->postConditionsToAdd->insert(itPostAdd, Predicate("handEmpty", args));     // handEmpty()
            itPostAdd = this->postConditionsToAdd->insert(itPostAdd, Predicate("clear", args));         // clear(A)

            itPostDel = this->postConditionsToDel->insert(itPostDel, Predicate("holding", args));       // holdind(A)
            //args[0] = args[1];
            itPostDel = this->postConditionsToDel->insert(itPostDel, Predicate("clear", reverseArgs));  // clear(B)
        }
        else if(name == "putOnTable") { // putOnTable(A)
            this->noArgs = 1;
            this->args = new int[noArgs];
            for(int i = 0; i < noArgs; i++)
                this->args[i] = args[i];

            itPre = this->preconditions->insert(itPre, Predicate("holding", args));                 // holding(A)

            itPostAdd = this->postConditionsToAdd->insert(itPostAdd, Predicate("onTable", args));   // onTable(A)
            itPostAdd = this->postConditionsToAdd->insert(itPostAdd, Predicate("clear", args));     // clear(A)
            itPostAdd = this->postConditionsToAdd->insert(itPostAdd, Predicate("handEmpty", args)); // handEmpty()

            itPostDel = this->postConditionsToDel->insert(itPostDel, Predicate("holding", args));   // holding(A)
        }
        else {
            std::cout << "Invalid Action name: " << name << std::endl;
        }
    }

    Action(Action *act) {
        Action(act->name, act->args);
    }

    Action() {
        this->noArgs = 0;
    }


    /* ---------------------------------------------------------------- */
    /* -------------------------GETTER-SETTER-------------------------- */
    /* ---------------------------------------------------------------- */


    inline std::set<Predicate> *getPreconditions() {
        return this->preconditions;
    }

    inline std::set<Predicate> *getPostconditionsToAdd() {
        return this->postConditionsToAdd;
    }

    inline std::set<Predicate> *getPostconditionsToDel() {
        return this->postConditionsToDel;
    }

    inline std::string getName() {
        return this->name;
    }

    inline int* getArgs() {
        return this->args;
    }

    inline int getNoArgs() {
        return this->noArgs;
    }

    /**
     * Getter.
     * @param indx
     * @return this->args[indx] if it exists, -1 otherwise
     */
    inline int getArg(int indx) {

        if(0 <= indx && indx <= this->noArgs) return this->args[indx];
        else {
            std::cout << "Invalid argument requested: action " << this->name << " has " << this->noArgs << "arguments; "
            << indx << "-th requested in Action::getArg." << std::endl;
            return -1;
        }
    }

    /**
     * Overload operator ==.
     * @param l left action
     * @param r right action
     * @return true if l == r, false otherwise
     */
    friend bool operator==(const Action& l, const Action& r);

    void operator=(const Action& other) {
        this->name = other.name;
        this->noArgs = other.noArgs;
        this->args = new int[other.noArgs];
        for(int i = 0; i < other.noArgs; i++) this->args[i] = other.args[i];
        this->preconditions = other.preconditions;
        this->postConditionsToDel = other.postConditionsToDel;
        this->postConditionsToAdd = other.postConditionsToAdd;
    }

    /**
     * Overload operator <.
     * @param l left action
     * @param r right action
     * @return true if l < r, false otherwise
     */
    friend bool operator<(const Action& l, const Action& r);

    /**
     * Overload operator <<.
     * @param o output stream
     * @param a the action to print
     * @return action(arg_1, ..., arg_n)
     */
    friend std::ostream& operator<<(std::ostream& o, const Action& a);
};

#endif //ROBOT_GOVERNANCE_PROJECT_ACTION_H
