//author: Stefan Caldararu
#include "alg.h"

//The Constructor
Alg::Alg()
{
    //TODO: probably need to do more in the constructor
}

void Alg::setGraph(Mspace& metric_space)
{
    std::size_t m = metric_space.getSize();
    metricSpace.setSize(m);
    for(int i = 0;i<m;i++)
        for(int j = 0;j<m;j++)
            metricSpace.setDistance(i,j,metric_space.getDistance(i,j));
}

void Alg::setServers(int numServers, std::vector <int> c)
{
    k = numServers;
    //TODO: check that this is written correctly.
    config.clear();
    coverage.clear();
    config.reserve(k);
    coverage.reserve(metricSpace.getSize());
    for(int i = 0; i<metricSpace.getSize();i++)
        coverage.push_back(-1);
    for(int i = 0; i<k; i++){
        config.push_back(c[i]);
        coverage[c[i]] = i;
    }
}
void Alg::moveServer(int server, int loc)
{
    coverage[config[server]] = -1;
    config[server] = loc;
    coverage[loc] = server;
}


//checks if the location is already covered by a server
bool Alg::checkIfCovered(int i){
    if(coverage[i] != -1)
        return true;
    else
        return false;
}
Alg::~Alg()
{}
