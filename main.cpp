
#include <iostream>
#include <fstream>
#include <string>

#include "Predicate.h"
#include "Action.h"
#include "Agent.h"

using namespace std;


#define NO_BLOCKS 3

int main() {
    std::cout << "Hello, Marco!" << std::endl;

    ofstream outFile;
    Environment env = Environment(NO_BLOCKS);
    Agent r = Agent(env, NO_BLOCKS);

    r.doQLearning(50);
    r.calculatePolicy();
    r.showPolicy();

    return 0;
}