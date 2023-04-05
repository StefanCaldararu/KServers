//author: Stefan Caldararu
#include "optAlg.h"
#include<cstdlib>
#include<iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dinic_max_flow.hpp>

OptAlg::OptAlg(): Alg()
{}

int OptAlg::runAlg(std::vector <int> Sigma, int inputLength)
{
    int cost = 0;
    //First we need to generate the network flow graph, and then we need to compute the network flow.


    return cost;
}

OptAlg::~OptAlg()
{}