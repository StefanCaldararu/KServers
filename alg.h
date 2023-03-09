//author: Stefan Caldararu

#include "mspace.h"

class Alg
{
    private:

    public:
        //The metric space
        Mspace metricSpace;
        //The constructor
        Alg(Mspace metric_space);
        //change the metric space we are currently using
        void setGraph(Mspace metric_space);
        //Run this algorithm on the metric space, with request sequence Simga. Return the cost.
        //virtual int runAlg(int * Sigma) = 0;
        //The destructor
        ~Alg();
};
