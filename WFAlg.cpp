//author: Stefan Caldararu
#include "WFAlg.h"
#include "RAII_Classes/mcfp.cpp"
#include<cstdlib>
#include<iostream>
#include "RAII_Classes/lemon_mcfp.cpp"

WFAlg::WFAlg(): Alg()
{}

int WFAlg::runAlg(std::vector <int> Sigma, int inputLength)
{
    std::vector<int> initConfig = config;
    int cost = 0;
    for(int i = 0;i<inputLength;i++){
        Lemon_Mcfp flowNetwork;
        std::vector<int> S;
        for(int j = 0;j<i+1;j++)
            S.push_back(Sigma[j]);
        int serverToMove = flowNetwork.setGraph(metricSpace, S, i+1, k, initConfig, config);
        //Now we have the server to move. 
        cost = cost+metricSpace.getDistance(config[serverToMove], Sigma[i]);
        moveServer(serverToMove, Sigma[i]);
        std::cout <<"MOVED SERVER " << std::to_string(i) <<" of " << std::to_string(inputLength) << "\n";
    }
    return cost;
}

WFAlg::~WFAlg()
{}