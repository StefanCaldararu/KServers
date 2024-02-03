//author: Stefan Caldararu
#include "doubleCoverageAlg.h"
#include<cstdlib>
#include<iostream>
//NOTE: This algorithm is the **ONLY** one that assumes that the metric space is not fully connected. Here, we are assuming that the metric space is a tree, and that the distances between nodes that are not actually connected are set to -1.
DoubleCoverageAlg::DoubleCoverageAlg(): Alg()
{}

int DoubleCoverageAlg::runAlg(std::vector <int> Sigma, int inputLength)
{
    int cost = 0;
    for(int i = 0;i<inputLength;i++){
        int input = Sigma[i];
        if(!checkIfCovered(input)){
            //First find which servers need to be moved, and then move them towards the input. 
            for(int j=0;j<k;j++){
                
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