//author: Stefan Caldararu
#ifndef GREEDYALG
#define GREEDYALG
#include "alg.h"

class GreedyAlg : public Alg
{
    private:

    public:
        GreedyAlg();
        int runAlg(std::vector <int> Sigma, int inputLength);
        ~GreedyAlg();
};
#endif