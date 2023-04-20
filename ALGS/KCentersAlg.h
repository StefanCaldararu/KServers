//author: Stefan Caldararu
#ifndef KCALG
#define KCALG
#include "alg.h"

class KCentersAlg : public Alg
{
    private:

    
    public:
    KCentersAlg();
    int runAlg(std::vector <int> Sigma, int inputLength);
    int checkMspace();
    ~KCentersAlg();
};
#endif