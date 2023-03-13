//author: Stefan Caldararu
#include "alg.h"

//The Constructor
Alg::Alg(Mspace& metric_space, int servers)
{
    k = servers;
    setGraph(metric_space);
}

void Alg::setGraph(Mspace& metric_space)
{
    std::size_t m = metric_space.getSize();
    metricSpace.setSize(m);
    for(int i = 0;i<m;i++)
        for(int j = 0;j<m;j++)
            metricSpace.setDistance(i,j,metric_space.getDistance(i,j));
}

//checks if the location is already covered by a server
bool Alg::checkIfCovered(int i, int config []){
    for(int j = 0;j<k;j++)
        if(config[j] == i)
            return true;
    return false;
}
Alg::~Alg()
{}
