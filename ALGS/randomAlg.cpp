//author: Stefan Caldararu
#include "randomAlg.h"
#include<cstdlib>
#include<iostream>

RandomAlg::RandomAlg(): Alg()
{}

int RandomAlg::runAlg(std::vector <int> Sigma, int inputLength)
{
    srand((unsigned) time(NULL));
    //TODO: there are some problems, check to make sure this works correctly!!!
    int random;
    int cost = 0;
    for(int i = 0; i<inputLength; i++){
        if(!checkIfCovered(Sigma[i])){
            random = rand() % k;
            cost += metricSpace.getDistance(config[random], Sigma[i]);
            moveServer(random, Sigma[i]);
        }
    }
    return cost;
}

RandomAlg::~RandomAlg()
{}