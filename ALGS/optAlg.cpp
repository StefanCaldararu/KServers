//author: Stefan Caldararu
#include "optAlg.h"
#include<cstdlib>
#include<iostream>
#include<chrono>

OptAlg::OptAlg(): Alg()
{}

int OptAlg::runAlg(std::vector <int> Sigma, int inputLength)
{
    // auto start = std::chrono::high_resolution_clock::now();
    flowNetwork.setGraph(metricSpace, Sigma, inputLength, k, config);
    int cost = flowNetwork.computeMCFP();
    // auto stop = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    // std::cout << duration.count() <<std::endl;
    return cost;
}

OptAlg::~OptAlg()
{}