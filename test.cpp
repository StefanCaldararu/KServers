#include <iostream>
#include "RAII_Classes/mcfp.cpp"
#include "mspace.h"
#include<vector>
int main() {
    Mspace metricSpace;
    metricSpace.setSize(4);
    metricSpace.setDistance(0,0,0);
    metricSpace.setDistance(0,1,3);
    metricSpace.setDistance(0,2,2);
    metricSpace.setDistance(1,0,3);
    metricSpace.setDistance(1,1,0);
    metricSpace.setDistance(1,2,5);
    metricSpace.setDistance(2,0,2);
    metricSpace.setDistance(2,1,5);
    metricSpace.setDistance(2,2,0);
    metricSpace.setDistance(3,3,0);

    metricSpace.setDistance(0,3,2);
    metricSpace.setDistance(3,0,2);
    metricSpace.setDistance(2,3,1);
    metricSpace.setDistance(3,2,1);
    metricSpace.setDistance(1,3,4);
    metricSpace.setDistance(3,1,4);

    std::vector<int> Sigma;
    Sigma.push_back(2);
    // Sigma.push_back(0);
    // Sigma.push_back(3);
    // Sigma.push_back(0);
    // Sigma.push_back(2);
    // Sigma.push_back(0);
    // Sigma.push_back(3);
    // Sigma.push_back(0);
    // Sigma.push_back(2);
    // Sigma.push_back(0);
    int inputLength = 1;
    int num_servers = 2;
    std::vector<int> init_config;
    init_config.push_back(0);
    init_config.push_back(1);
    Mcfp calc;
    calc.setGraph(metricSpace, Sigma, inputLength, num_servers, init_config);
    int cost = calc.computeMCFP();
    std::cout << cost << std::endl;
    return 0;
}