//author: Stefan Caldararu

#ifndef OPTALG
#define OPTALG
#include "alg.h"
#include "RAII_Classes/mcfp.cpp"
#include "RAII_Classes/lemon_mcfp.cpp"

class OptAlg : public Alg
{
    private:

    public:
    OptAlg();
    Lemon_Mcfp flowNetwork;
    int runAlg(std::vector <int> Sigma, int inputLength);
    ~OptAlg();
};

#endif