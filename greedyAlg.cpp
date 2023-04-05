//author: Stefan Caldararu
#include "greedyAlg.h"
#include<cstdlib>
#include<iostream>

GreedyAlg::GreedyAlg(): Alg()
{}

int GreedyAlg::runAlg(std::vector <int> Sigma, int inputLength)
{
    int cost = 0;
    for(int i = 0; i<inputLength;i++){
        int input = Sigma[i];
        if(!checkIfCovered(input)){
            //FIXME: we always move the server w/ smallest ID number to the location in the event of a tie, should we do something else?
            int closest = 0;
            int closest_dist = metricSpace.getDistance(input, config[0]);
            for(int j = 1; j<k; j++){
                int dist = metricSpace.getDistance(input, config[j]);
                if(closest_dist>dist){
                    closest = j;
                    closest_dist = dist;
                }
            }
            cost = cost+closest_dist;
            moveServer(closest, input);
        }
    }
    return cost;
}

GreedyAlg::~GreedyAlg()
{}