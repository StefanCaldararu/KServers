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
            //an array of paths, where the the index represents the "from" node and the path is to the input node / request. If the vector is empty, then the node has another server that is on it's path that is closer than it to the input.
            std::vector<std::vector<int>> paths;
            for(int j = 0;j<k;j++)
                paths.push_back(std::vector<int>());
            //First find which servers need to be moved, and then move them towards the input. 
            for(int j=0;j<k;j++){
                //get the path between the server and the input, as a vector.
                paths[j] = metricSpace.findTreePathDFS(config[j], input);
                for(int l = 1;l<paths[j].size();l++){
                    if(coverage[paths[j][l]].size() > 1){
                        //if the node is covered, than we don't want to set the path into the paths variable. otherwise we do
                        paths[j].clear();
                        break;
                    }
                }
                //if the current node we are on has another server on it, then we only want to add one of the two to the paths variable.
            }
            //now we have a list of servers that can be moved towards the input. We want to start moving them towards the input, but we need to recheck that their paths to the input are "clear" after each step.
            int steps = 1;
            while(!checkIfCovered(input)){
                for(int j = 0;j<k;j++){
                    if(paths[j].size() == 0)
                        continue;
                    //first, move all the servers one step closer to the input (that have paths)
                    moveServer(j, paths[j][steps]);
                    //add one to the cost
                    cost++;
                }
                steps++;
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