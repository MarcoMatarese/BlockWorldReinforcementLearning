//
// Created by Marco Matarese on 18/06/2019.
//

#ifndef ROBOT_GOVERNANCE_PROJECT_PREDICATE_H
#define ROBOT_GOVERNANCE_PROJECT_PREDICATE_H


#include <string>
#include <iostream>


class Predicate {


private:


    std::string name;
    int noArgs;
    int *args = nullptr;


public:


    /* ---------------------------------------------------------------- */
    /* --------------------------CONSTRUCTORS-------------------------- */
    /* ---------------------------------------------------------------- */


    Predicate() {
        this->noArgs = 0;
    }

    Predicate(std::string name, int *args) {

        this->name = name;

        if(name == "on") {
            this->noArgs = 2;
            this->args = new int[noArgs];
            for(int i = 0; i < noArgs; i++)
                this->args[i] = args[i];
        }
        else if(name == "holding") {
            this->noArgs = 1;
            this->args = new int[noArgs];
            this->args[0] = args[0];
        }
        else if(name == "onTable") {
            this->noArgs = 1;
            this->args = new int[noArgs];
            this->args[0] = args[0];
        }
        else if(name == "clear") {
            this->noArgs = 1;
            this->args = new int[noArgs];
            this->args[0] = args[0];
        }
        else if(name == "handEmpty") {
            this->noArgs = 0;
        }
        else {
            std::cout << "Invalid Predicate name: " << name << std::endl;
        }
    }

    Predicate(std::string n, int noArg, int *arg) {
        /*this->name = n;
        this->noArgs = noArg;
        this->args = new int[noArg];
        for(int i = 0; i < noArg; i++) this->args[i] = arg[i];*/
        Predicate(n, arg);
        if(this->noArgs != noArg) Predicate();
    }

    Predicate(std::string n) {
        this->name = n;
        this->noArgs = 0;
    }

    Predicate(Predicate *pred) {
        Predicate(pred->name, pred->args);
    }

    /*~Predicate() {
        delete[] this->args;
    }*/


    /* ---------------------------------------------------------------- */
    /* -------------------------GETTER-SETTER-------------------------- */
    /* ---------------------------------------------------------------- */


    inline std::string getName() {
        return this->name;
    }

    /**
     * Getter.
     * @return this->args[1] if exists, -1 otherwise
     */
    inline int getSecondArg() {
        if(this->noArgs == 2) return this->args[1];
        else return -1;
    }

    inline int getArg(int indx) {

        if(0 <= indx && indx <= this->noArgs) return this->args[indx];
        else {
            std::cout << "Invalid argument requested: predicate " << this->name << " has " << this->noArgs << "arguments; "
                      << indx << "-th requested in Predicate::getArg." << std::endl;
            return -1;
        }
    }

    /*
    inline bool operator==(const Predicate& other) const {

        if(this->name != other.name) return false;
        if(this->noArgs != other.noArgs) return false;
        for(int i = 0; i < this->noArgs; i++)
            if(this->args[i] != other.args[i]) return false;
        return true;
    }
     */


    /* ---------------------------------------------------------------- */
    /* ----------------------------METHODS----------------------------- */
    /* ---------------------------------------------------------------- */


    inline bool isEmptyPredicate() {
        return this->name == "emptyPredicate";
    }

    /**
     * Overload operator ==.
     * @param l left predicate
     * @param r right predicate
     * @return true if l == r, false otherwise
     */
    friend bool operator==(const Predicate& l, const Predicate& r);

    /*
    inline bool operator<(const Predicate& other) const {

        if(this == &other) return false;                        // clearly false

        if(this->name == other.name) {               // have same name
            if(this->noArgs == other.noArgs) {
                for(int i = 0; i < this->noArgs; i++)
                    if(this->args[i] != other.args[i])          // induce an lexicographic order on arguments
                        return this->args[i] < other.args[i];
            }
            else
                return this->noArgs < other.noArgs;
        }

        return this->name.compare(other.name) < 0;              // if they have not same name, compare on it
    }
    */

    friend bool operator<(const Predicate& r, const Predicate& l);

    /**
     * Overload operator <<.
     * @param o output stream
     * @param p the predicate to print
     * @return predicate(arg_1, ..., arg_n)
     */
    friend std::ostream& operator<<(std::ostream& o, const Predicate& p);
};



#endif //ROBOT_GOVERNANCE_PROJECT_PREDICATE_H
