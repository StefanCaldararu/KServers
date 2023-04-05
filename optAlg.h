//author: Stefan Caldararu

#ifndef OPTALG
#define OPTALG
#include "alg.h"

class OptAlg : public Alg
{
    private:

    public:
    OptAlg();
    int runAlg(std::vector <int> Sigma, int inputLength);
    ~OptAlg();
};

#endif