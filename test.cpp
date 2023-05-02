#include "RAII_Classes/mcfp.cpp"
#include "mspace.h"
#include <stdio.h>
#include <vector>
#include <iostream>
  
int main()
{
    Mcfp flowNetwork;
    Mspace metricSpace;
    metricSpace.setSize(2);
    metricSpace.setDistance(0,1,1);
    metricSpace.setDistance(0,0,0);
    metricSpace.setDistance(1,0,1);
    metricSpace.setDistance(1,1,0);
    std::vector<int> Sigma;
    Sigma.push_back(1);
    int IL = 1;
    int num_servers = 1;
    std::vector<int> config;
    config.push_back(0);
    flowNetwork.setGraph_WFA(metricSpace, Sigma, IL, num_servers, config, config);
    flowNetwork.moveServer(0, 1);
    flowNetwork.add_input(0);


    
    return 0;
}