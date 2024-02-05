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
        coverage.push_back(std::vector<int>());
    for(int i = 0; i<k; i++){
        config.push_back(c[i]);
        coverage[c[i]].push_back(i);
    }
}
void Alg::moveServer(int server, int loc)
{
    // for(int i = 0;i<coverage[config[server]].size();i++){
    //     if(coverage[config[server]][i] == server){
    //         coverage[config[server]].erase(coverage[config[server]].begin() + i);
    //         break;
    //     }
    // }
    coverage[config[server]].erase(std::remove(coverage[config[server]].begin(), coverage[config[server]].end(), server), coverage[config[server]].end());  
    config[server] = loc;

    coverage[loc].push_back(server);
}


//checks if the location is already covered by a server
bool Alg::checkIfCovered(int i){
    if(coverage[i].size()!=0)
        return true;
    else
        return false;
}
Alg::~Alg()
{}
