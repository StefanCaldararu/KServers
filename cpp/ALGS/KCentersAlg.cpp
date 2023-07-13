//author: Stefan Caldararu
#include "KCentersAlg.h"
#include<cstdlib>
#include<iostream>

KCentersAlg::KCentersAlg(): Alg()
{}

int KCentersAlg::runAlg(std::vector <int> Sigma, int inputLength)
{
    if(checkMspace() == -1)
        return -1;
    int cost = 0;
    for(int i = 0;i<inputLength;i++){
        int input = Sigma[i];
        if(!checkIfCovered(input)){
            int ps = metricSpace.getSize()/k;
            int r = metricSpace.getSize()-ps*k;
            int ID = -1;
            while(input>=0){
                ID++;
                input = input -ps;
                if(r>0){
                    r--;
                    input --;
                }
            }
            cost = cost+metricSpace.getDistance(config[ID], Sigma[i]);
            moveServer(ID, Sigma[i]);
        }       
    }
    
    return cost;
}
int KCentersAlg::checkMspace(){
    for(int i = 0;i<metricSpace.getSize();i++)
        for(int j = 0;j<metricSpace.getSize();j++)
            if(metricSpace.getDistance(i,j) != abs(i-j))
                return -1;
    return 0;
}

KCentersAlg::~KCentersAlg()
{}