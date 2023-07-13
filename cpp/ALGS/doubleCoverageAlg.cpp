//author: Stefan Caldararu
#include "doubleCoverageAlg.h"
#include<cstdlib>
#include<iostream>

DoubleCoverageAlg::DoubleCoverageAlg(): Alg()
{}

int DoubleCoverageAlg::runAlg(std::vector <int> Sigma, int inputLength)
{
    if(checkMspace() == -1)
        return -1;
    int cost = 0;
    for(int i = 0;i<inputLength;i++){
        int input = Sigma[i];
       

        if(!checkIfCovered(input)){
            std::vector<int> closest = findClosestID(input);
            if(closest[0] == -1){
                cost = cost+metricSpace.getDistance(input, config[closest[1]]);
                moveServer(closest[1], input);
                config[closest[1]] = input;
            }
            else if(closest[1] == -1){
                cost = cost+metricSpace.getDistance(input, config[closest[0]]);
                moveServer(closest[0], input);
                config[closest[0]] = input;
            }
            else if(metricSpace.getDistance(config[closest[0]], input)<metricSpace.getDistance(config[closest[1]],input)){
                int dist = metricSpace.getDistance(config[closest[0]], input);
                cost = cost+2*dist;
                moveServer(closest[1], config[closest[1]]-dist);
                moveServer(closest[0], input);
            }
            else{
                int dist = metricSpace.getDistance(config[closest[1]], input);
                cost = cost+2*dist;
                moveServer(closest[0], config[closest[0]]+dist);
                moveServer(closest[1], input);
            }
        }
    }
    
    return cost;
}
int DoubleCoverageAlg::checkMspace(){
    for(int i = 0;i<metricSpace.getSize();i++)
        for(int j = 0;j<metricSpace.getSize();j++)
            if(metricSpace.getDistance(i,j) != abs(i-j))
                return -1;
    return 0;
}

std::vector<int> DoubleCoverageAlg::findClosestID(int input){
    std::vector<int> ID = {-1,-1};
    for(int i = 0;i<k;i++){
        if(config[i]<input && (ID[0] == -1 || metricSpace.getDistance(ID[0], input)>metricSpace.getDistance(config[i],input)))
            ID[0] = i;
        else if(config[i]>input && (ID[1] == -1 || metricSpace.getDistance(ID[1], input)>metricSpace.getDistance(config[i], input)))
            ID[1] = i;
    }
    return ID;
}

DoubleCoverageAlg::~DoubleCoverageAlg()
{}