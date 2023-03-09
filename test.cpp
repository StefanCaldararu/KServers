//author: Stefan Caldararu

//#include "mspace.h"
#include "alg.h"
#include <iostream>

int main()
{
    Mspace mSpace;
    mSpace.setSize(5);
    mSpace.setDistance(1,1,25);
    Alg algorithm(mSpace);
    std::cout << algorithm.metricSpace.getDistance(1,1);
    // std::cout << mSpace.getDistance(1,1);
    return 0;
}