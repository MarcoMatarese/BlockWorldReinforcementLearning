//
// Created by Marco Matarese on 18/06/2019.
//

#include "Predicate.h"


bool operator<(const Predicate& l, const Predicate& r) {
    if(l == r) return false;                        // clearly false

    if(l.name == r.name) {                          // have same name
        if(l.noArgs == r.noArgs) {
            for(int i = 0; i < l.noArgs; i++)
                if(l.args[i] != r.args[i])          // induce an lexicographic order on arguments
                    return l.args[i] < r.args[i];
        }
        else
            return l.noArgs < r.noArgs;
    }

    return l.name.compare(r.name) < 0;              // if they have not same name, compare on it
}

std::ostream& operator<<(std::ostream& o, const Predicate& p) {
    o << p.name + "(";
    for(int i = 0; i < p.noArgs; i++)
        o << p.args[i];
    o << ")";
    return o;
}

bool operator==(const Predicate& l, const Predicate& r) {
    if(l.name != r.name) return false;
    if(l.noArgs != r.noArgs) return false;
    for(int i = 0; i < l.noArgs; i++)
        if(l.args[i] != r.args[i]) return false;
    return true;
}