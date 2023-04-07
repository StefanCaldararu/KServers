//author: Stefan Caldararu
#include "optAlg.h"
#include<cstdlib>
#include<iostream>

OptAlg::OptAlg(): Alg()
{}

int OptAlg::runAlg(std::vector <int> Sigma, int inputLength)
{
    flowNetwork.setGraph(metricSpace, Sigma, inputLength, k, config);
    int cost = flowNetwork.computeMCFP();
    return cost;
}

OptAlg::~OptAlg()
{}