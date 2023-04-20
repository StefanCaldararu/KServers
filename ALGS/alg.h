//author: Stefan Caldararu

#ifndef ALG
#define ALG

#include "../mspace.h"
#include <vector>



class Alg
{
    private:

    public:
        bool checkIfCovered(int i);
        //The number of servers
        int k;
        //The integer location of each server
        std::vector <int> config;
        //The points that are covered have server id, the ones that don't have a -1.
        std::vector <int> coverage;
        //The metric space
        Mspace metricSpace;
        //The constructor
        Alg();
        //set the configuration of servers.
        void setServers(int numServers, std::vector <int> c);
        //move server with id server to location loc
        void moveServer(int server, int loc);
        //change the metric space we are currently using
        void setGraph(Mspace& metric_space);
        //Run this algorithm on the metric space, with request sequence Simga. Return the cost.
        virtual int runAlg(std::vector <int> Sigma, int inputLength) = 0;
        //The destructor
        ~Alg();
};

#endif