//author: Stefan Caldararu

#ifndef WFALG
#define WFALG
#include "alg.h"

class WFAlg : public Alg
{
    private:

    public:
    WFAlg();
    int runAlg(std::vector <int> Sigma, int inputLength);
    ~WFAlg();
};

#endif