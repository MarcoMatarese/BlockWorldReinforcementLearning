//
// Created by Marco Matarese on 18/06/2019.
//


#include "Action.h"


/**
 * Overload operator ==.
 * @param l left action
 * @param r right action
 * @return true if l == r, false otherwise
 */
bool operator==(const Action& l, const Action& r) {
    if(l.name != r.name) return false;
    if(l.noArgs != r.noArgs) return false;
    for(int i = 0; i < l.noArgs; i++) {
        if (l.args[i] != r.args[i]) {
            return false;
        }
    }
    return true;
}

/**
 * Overload operator <.
 * @param l left action
 * @param r right action
 * @return true if l < r, false otherwise
 */
bool operator<(const Action& l, const Action& r) {

    if(l.name != r.name)
        if(l.name < r.name) return true;        // compare on their names
    if(l.noArgs != r.noArgs) {
        if (l.noArgs < r.noArgs) return true;   // compare on non arguments
        else return false;
    }
    for(int i = 0; i < l.noArgs; i++)
        if(l.args[i] < r.args[i]) return true;  // compare on their arguments
    return false;                               // they are equal
}

/**
 * Overload operator <<.
 * @param o output stream
 * @param a the action to print
 * @return action(arg_1, ..., arg_n)
 */
std::ostream& operator<<(std::ostream& o, const Action& a) {
    o << a.name + "(";
    for(int i = 0; i < a.noArgs; i++)
        o << a.args[i];
    o << ")";
    return o;
}