//author: Stefan Caldararu
#ifndef RANDALG
#define RANDALG
#include "alg.h"

class RandomAlg : public Alg
{
    private:

    public:
    RandomAlg();
    int runAlg(std::vector <int> Sigma, int inputLength);
    ~RandomAlg();
};
#endif