//author: Stefan Caldararu

#include "randomAlg.h"
#include <iostream>

int main()
{
    Mspace mSpace;
    mSpace.setSize(2);
    mSpace.setDistance(0,0,1);
    mSpace.setDistance(0,1,1);
    mSpace.setDistance(1,0,1);
    mSpace.setDistance(1,1,1);
    RandomAlg algorithm(mSpace, 2);
    int Sigma [] = {0,1,1,0};
    int config[] = {0,0};
    std::cout << algorithm.runAlg(Sigma, 4, config);
    // std::cout << mSpace.getDistance(1,1);
    return 0;
}