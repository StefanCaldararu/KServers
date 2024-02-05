//author: Stefan Caldararu
#include "doubleCoverageTreeAlg.h"
#include<cstdlib>
#include<iostream>
//NOTE: This algorithm is the **ONLY** one that assumes that the metric space is not fully connected. Here, we are assuming that the metric space is a tree, and that the distances between nodes that are not actually connected are set to -1.
DoubleCoverageTreeAlg::DoubleCoverageTreeAlg(): Alg()
{}

int DoubleCoverageTreeAlg::runAlg(std::vector <int> Sigma, int inputLength)
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
            //a vector holding the locations that are doubled-up on servers, but already have a server going to the input on them.
            std::vector<int> doubledUp;
            for(int j=0;j<k;j++){
                //check if there are two servers at the starting location, and if the location we are in is already in doubledUp. If it is, then we clear this path and move on to the next server. If it is not in doubledUp, then we add it to doubledUp and continue to the next line.
                if(coverage[config[j]].size() > 1 && std::find(doubledUp.begin(), doubledUp.end(), config[j]) == doubledUp.end()){
                    doubledUp.push_back(config[j]);
                }
                else if(coverage[config[j]].size() > 1){
                    continue;
                }
                //get the path between the server and the input, as a vector.
                paths[j] = metricSpace.findTreePathDFS(config[j], input);
                for(int l = 1;l<paths[j].size();l++){
                    if(coverage[paths[j][l]].size() >= 1){
                        //if the node is covered, than we don't want to set the path into the paths variable. otherwise we do
                        paths[j].clear();
                        break;
                    }
                    if(paths[j][l] == input)
                        break;
                }
            }
            //now we have a list of servers that can be moved towards the input. We want to start moving them towards the input, but we need to recheck that their paths to the input are "clear" after each step.
            int steps = 1;
            while(!checkIfCovered(input)){
                for(int j = 0;j<k;j++){
                    if(paths[j].size() == 0)
                        continue;
                    //first, move all the servers one step closer to the input (that have paths)
                    cost+= metricSpace.getDistance(config[j], paths[j][steps]);
                    moveServer(j, paths[j][steps]);
                }
                //check if there are any servers that are now doubled up. If there are, then we need to remove one of them from the paths, and then continue.
                for(int j = 0;j<metricSpace.getSize();j++)
                    for(int l = 1;l<coverage[j].size();l++)
                        paths[coverage[j][l]].clear();
                //also check if there are any servers that have entered another servers path. If there are, then we need to remove the server from the path, and then continue.
                for(int j = 0;j<k;j++){
                    if(paths[j].size() == 0)
                        continue;
                    if(paths[j][steps] == input)
                        break;
                    for(int l = steps+1;l<paths[j].size();l++){
                        if(coverage[paths[j][l]].size() >= 1){
                            paths[j].clear();
                            break;
                        }
                        if(paths[j][l] == input)
                            break;
                    }
                }
                steps++;
            }
        }
    }
    
    return cost;
}

int DoubleCoverageTreeAlg::checkMspace(){
    for(int i = 0;i<metricSpace.getSize();i++)
        for(int j = 0;j<metricSpace.getSize();j++)
            if(metricSpace.getDistance(i,j) != abs(i-j))
                return -1;
    return 0;
}

std::vector<int> DoubleCoverageTreeAlg::findClosestID(int input){
    std::vector<int> ID = {-1,-1};
    for(int i = 0;i<k;i++){
        if(config[i]<input && (ID[0] == -1 || metricSpace.getDistance(ID[0], input)>metricSpace.getDistance(config[i],input)))
            ID[0] = i;
        else if(config[i]>input && (ID[1] == -1 || metricSpace.getDistance(ID[1], input)>metricSpace.getDistance(config[i], input)))
            ID[1] = i;
    }
    return ID;
}

DoubleCoverageTreeAlg::~DoubleCoverageTreeAlg()
{}