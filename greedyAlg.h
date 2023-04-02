//author: Stefan Caldararu

#include "alg.h"

class GreedyAlg : public Alg
{
    private:

    public:
    GreedyAlg(Mspace& metric_space, int servers);
    int runAlg(int Sigma[], int inputLength, int config[]);
    ~GreedyAlg();
};