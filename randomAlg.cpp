//author: Stefan Caldararu
#include "randomAlg.h"
#include<cstdlib>
#include<iostream>

RandomAlg::RandomAlg(Mspace& metric_space, int servers): Alg(metric_space, servers)
{}

int RandomAlg::runAlg(int Sigma[], int inputLength, int config[])
{
    srand((unsigned) time(NULL));

    int random;
    int cost = 0;
    for(int i = 0; i<inputLength; i++){
        if(!Alg::checkIfCovered(Sigma[i], config)){
            random = rand() % k;
            cost += metricSpace.getDistance(config[random], Sigma[i]);
            config[random] = Sigma[i];
        }
    }
    return cost;
}

RandomAlg::~RandomAlg()
{}