//author: Stefan Caldararu
#include "alg.h"

//The Constructor
Alg::Alg(Mspace& metric_space)
{
    setGraph(metric_space);
    //The destructor is being called here for some metric space, but one that
    //nothing allocated....
}

void Alg::setGraph(Mspace& metric_space)
{
    std::size_t m = metric_space.getSize();
    metricSpace.setSize(m);
    for(int i = 0;i<m;i++)
        for(int j = 0;j<m;j++)
            metricSpace.setDistance(i,j,metric_space.getDistance(i,j));
}

Alg::~Alg()
{
    
}
