//author: Stefan Caldararu
#ifndef DCALG
#define DCALG
#include "alg.h"

class DoubleCoverageAlg : public Alg
{
    private:

    
    public:
    std::vector<int> findClosestID(int input);
    DoubleCoverageAlg();
    int runAlg(std::vector <int> Sigma, int inputLength);
    int checkMspace();
    ~DoubleCoverageAlg();
};
#endif