//author: Stefan Caldararu
#ifndef LDCALG
#define LDCALG
#include "alg.h"

class LazyDoubleCoverageAlg : public Alg
{
    private:
        std::vector<int> findClosestID(int input);
        int checkMspace();
        //The virtual integer location of each server
        std::vector <int> virtualConfig;
        //The virtual points that are covered have server id, the ones that don't have a -1.
        std::vector <int> virtualCoverage;
        //move virtual server with id server to location loc
        void moveVirtualServer(int server, int loc);
        bool checkIfVirtuallyCovered(int i);
    public:
        LazyDoubleCoverageAlg();
        int runAlg(std::vector <int> Sigma, int inputLength);
        ~LazyDoubleCoverageAlg();
};
#endif