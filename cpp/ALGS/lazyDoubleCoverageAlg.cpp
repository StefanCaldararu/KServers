//author: Stefan Caldararu
#include "lazyDoubleCoverageAlg.h"
#include<cstdlib>
#include<iostream>

LazyDoubleCoverageAlg::LazyDoubleCoverageAlg(): Alg()
{}

int LazyDoubleCoverageAlg::runAlg(std::vector <int> Sigma, int inputLength)
{
    if(checkMspace() == -1)
        return -1;
    int cost = 0;
    //set the virtual server configuration 
    virtualConfig.clear();
    virtualCoverage.clear();
    for(int i = 0;i<config.size();i++)
        virtualConfig.push_back(config[i]);
    for(int i = 0;i<coverage.size();i++)
        virtualCoverage.push_back(coverage[i]);

    for(int i = 0;i<inputLength;i++){
        int input = Sigma[i];
       

        if(!checkIfCovered(input)){
            if(checkIfVirtuallyCovered(input)){
                //virtually covered, so move the appropriate server.
                cost = cost+metricSpace.getDistance(input, config[virtualCoverage[input]]);
                moveServer(virtualCoverage[input], input);
                //FIXME: this line shouldn't be necessary...
                config[virtualCoverage[input]] = input;
                //no need to move virtual server, as it is in the right spot already.
            }
            else{
                std::vector<int> closest = findClosestID(input);
                if(closest[0] == -1){
                    //Still move the closest server, but update virtual stuff.
                    cost = cost+metricSpace.getDistance(input, config[closest[1]]);
                    moveServer(closest[1], input);
                    config[closest[1]] = input;
                    moveVirtualServer(closest[1], input);
                    virtualConfig[closest[1]] = input;
                }
                else if(closest[1] == -1){
                    //Still move the closest server, but update virtual stuff.
                    cost = cost+metricSpace.getDistance(input, config[closest[0]]);
                    moveServer(closest[0], input);
                    config[closest[0]] = input;
                    moveVirtualServer(closest[0], input);
                    virtualConfig[closest[0]] = input;
                }
                else if(metricSpace.getDistance(virtualConfig[closest[0]], input)<metricSpace.getDistance(virtualConfig[closest[1]],input) || (
                    metricSpace.getDistance(config[closest[0]],input)<metricSpace.getDistance(config[closest[1]],input) && metricSpace.getDistance(virtualConfig[closest[0]], input)==metricSpace.getDistance(virtualConfig[closest[1]],input)
                )){
                    //only move the closest server...
                    int dist = metricSpace.getDistance(config[closest[0]], input);
                    cost = cost+dist;
                    //moveServer(closest[1], config[closest[1]]-dist);
                    moveServer(closest[0], input);
                    int virtualDist = metricSpace.getDistance(virtualConfig[closest[0]], input);
                    moveVirtualServer(closest[1], virtualConfig[closest[1]]-virtualDist);
                    moveVirtualServer(closest[0], input);
                }
                else{
                    int dist = metricSpace.getDistance(config[closest[1]], input);
                    cost = cost+dist;
                    //moveServer(closest[0], config[closest[0]]+dist);
                    moveServer(closest[1], input);
                    int virtualDist = metricSpace.getDistance(virtualConfig[closest[1]], input);
                    moveVirtualServer(closest[0], virtualConfig[closest[1]]-virtualDist);
                    moveVirtualServer(closest[1], input);
                }
            }
        }
    }
    
    return cost;
}
int LazyDoubleCoverageAlg::checkMspace(){
    for(int i = 0;i<metricSpace.getSize();i++)
        for(int j = 0;j<metricSpace.getSize();j++)
            if(metricSpace.getDistance(i,j) != abs(i-j))
                return -1;
    return 0;
}

std::vector<int> LazyDoubleCoverageAlg::findClosestID(int input){
    std::vector<int> ID = {-1,-1};
    for(int i = 0;i<k;i++){
        if(virtualConfig[i]<input && (ID[0] == -1 || metricSpace.getDistance(ID[0], input)>metricSpace.getDistance(virtualConfig[i],input)))
            ID[0] = i;
        else if(virtualConfig[i]>input && (ID[1] == -1 || metricSpace.getDistance(ID[1], input)>metricSpace.getDistance(virtualConfig[i], input)))
            ID[1] = i;
    }
    return ID;
}

void LazyDoubleCoverageAlg::moveVirtualServer(int server, int loc)
{
    virtualCoverage[virtualConfig[server]] = -1;
    virtualConfig[server] = loc;
    virtualCoverage[loc] = server;
}

//checks if the location is already covered by a virtualserver
bool LazyDoubleCoverageAlg::checkIfVirtuallyCovered(int i){
    if(virtualCoverage[i] != -1)
        return true;
    else
        return false;
}

LazyDoubleCoverageAlg::~LazyDoubleCoverageAlg()
{}