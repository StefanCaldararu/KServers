//author: Stefan Caldararu
#ifndef DCTALG
#define DCTALG
#include "alg.h"

class DoubleCoverageTreeAlg : public Alg
{
    private:
        std::vector<int> findClosestID(int input);
        int checkMspace();
    public:
        DoubleCoverageTreeAlg();
        int runAlg(std::vector <int> Sigma, int inputLength);
        
        ~DoubleCoverageTreeAlg();
};
#endif