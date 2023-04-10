//author: Stefan Caldararu

#ifndef OPTALG
#define OPTALG
#include "alg.h"
#include "RAII_Classes/mcfp.cpp"

class WFAlg : public Alg
{
    private:

    public:
    WFAlg();
    Mcfp flowNetwork;
    int runAlg(std::vector <int> Sigma, int inputLength);
    ~WFAlg();
};

#endif