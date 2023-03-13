//author: Stefan Caldararu

#include "alg.h"

class RandomAlg : public Alg
{
    private:

    public:
    RandomAlg(Mspace& metric_space, int servers);
    int runAlg(int Sigma[], int inputLength, int config[]);
    ~RandomAlg();
};