//author: Stefan Caldararu

#include "mspace.h"

class Alg
{
    private:

    public:
        bool checkIfCovered(int i, int config[]);
        //The number of servers
        int k;
        //The metric space
        Mspace metricSpace;
        //The constructor
        Alg(Mspace& metric_space, int servers);
        //change the metric space we are currently using
        //TODO: why do we need to pass with an &?
        void setGraph(Mspace& metric_space);
        //Run this algorithm on the metric space, with request sequence Simga. Return the cost.
        virtual int runAlg(int Sigma[], int inputLength, int config[]) = 0;
        //The destructor
        ~Alg();
};
