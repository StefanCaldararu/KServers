//author: Stefan Caldararu

#ifndef OPTALG
#define OPTALG
#include "alg.h"
#include "RAII_Classes/mcfp.cpp"

class OptAlg : public Alg
{
    private:

    public:
    OptAlg();
    Mcfp flowNetwork;
    int runAlg(std::vector <int> Sigma, int inputLength);
    ~OptAlg();
};

#endif