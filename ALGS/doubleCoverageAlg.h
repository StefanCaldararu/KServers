//author: Stefan Caldararu
#ifndef DCALG
#define DCALG
#include "alg.h"

class DoubleCoverageAlg : public Alg
{
    private:
        std::vector<int> findClosestID(int input);
        int checkMspace();
    public:
        DoubleCoverageAlg();
        int runAlg(std::vector <int> Sigma, int inputLength);
        ~DoubleCoverageAlg();
};
#endif