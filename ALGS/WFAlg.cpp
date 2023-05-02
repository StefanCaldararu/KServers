//author: Stefan Caldararu
#include "WFAlg.h"
#include "../RAII_Classes/mcfp.cpp"
#include<cstdlib>
#include<iostream>
#include "../RAII_Classes/lemon_mcfp.cpp"

WFAlg::WFAlg(): Alg()
{}

int WFAlg::runAlg(std::vector <int> Sigma, int inputLength)
{
    std::vector<int> init;
    init.push_back(Sigma[0]);
    Mcfp flowNetwork;
    flowNetwork.setGraph_WFA(metricSpace, init,1, k, config, config );
    int cost = 0;
    int serverToMove = flowNetwork.compute();
    cost = cost+metricSpace.getDistance(config[serverToMove],Sigma[0]);
    moveServer(serverToMove, Sigma[0]);
    flowNetwork.moveServer(serverToMove, Sigma[0]);
    for(int i = 1;i<inputLength;i++){
        flowNetwork.add_input(Sigma[i]);
        serverToMove = flowNetwork.compute();
        cost = cost+metricSpace.getDistance(config[serverToMove],Sigma[i]);
        moveServer(serverToMove, Sigma[i]);
        flowNetwork.moveServer(serverToMove,Sigma[i]);
    }
    


    // auto start = std::chrono::high_resolution_clock::now();
    // std::vector<int> initConfig = config;
    // int cost = 0;
    // for(int i = 0;i<inputLength;i++){
    //     Mcfp flowNetwork;
    //     std::vector<int> S;
    //     for(int j = 0;j<i+1;j++)
    //         S.push_back(Sigma[j]);
    //     int serverToMove = flowNetwork.setGraph(metricSpace, S, i+1, k, initConfig, config);
    //     //Now we have the server to move. 
    //     cost = cost+metricSpace.getDistance(config[serverToMove], Sigma[i]);
    //     moveServer(serverToMove, Sigma[i]);
    // }
    // auto stop = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    //std::cout << duration.count() <<std::endl;
    return cost;
}

WFAlg::~WFAlg()
{}