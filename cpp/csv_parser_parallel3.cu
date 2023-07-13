//FIXME: for this to work, we will need to rewrite a lot of the previous calsses to be able to be run by __devices__, and to not use certain cpp features (vecs, for example).


#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <vector>
#include <fstream>

#include "RAII_Classes/getInput.cpp"
#include "RAII_Classes/writeOutput.cpp"
#include "RAII_Classes/out.h"
#include "RAII_Classes/buffer.hpp"

#include "ALGS/WFAlg.h"
#include "ALGS/randomAlg.h"
#include "ALGS/optAlg.h"
#include "ALGS/greedyAlg.h"
#include "ALGS/doubleCoverageAlg.h"
#include "ALGS/KCentersAlg.h"

#define N 10000000

struct cost{
    int location;
    int rand;
    int greedy;
    int OPT;
    int WFA;
    int DC;
    int KC;
    std::vector<int> input;
};

__global__ void compute(cost *out, Mspace *metric_space, int k, int n){
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if(tid>n)
        return;
    
    RandomAlg ralg;
    GreedyAlg galg;
    OptAlg oalg;
    WFAlg walg;
    DoubleCoverageAlg dalg;
    KCentersAlg kalg;

    std::vector<int> server_locations;
    server_locations.reserve(k);
    for(int j = 0;j<k;j++)
        server_locations.push_back(j);
    
    int size = out[tid].input.size();
    ralg.setGraph(*metric_space);
    ralg.setServers(k, server_locations);
    out[tid].rand = ralg.runAlg(out[tid].input, size);

    
}