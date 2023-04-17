//author: Stefan Caldararu

#ifndef WFALG
#define WFALG
#include "alg.h"
#include "RAII_Classes/mcfp.cpp"

class WFAlg : public Alg
{
    private:

    public:
    WFAlg();
    int runAlg(std::vector <int> Sigma, int inputLength);
    ~WFAlg();
};

#endif