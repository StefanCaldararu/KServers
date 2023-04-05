//author: Stefan Caldararu

#ifndef MCFP
#define MCFP

#include <vector>
#include "../mspace.h"

struct edge{
    int cost;
    int flow;
};

class Mcfp
{
    private:
        std::vector<std::vector<edge> > graph;
    public:
        Mcfp(){}
        void setGraph(Mspace & metricSpace, std::vector<int> Sigma, int inputLength, int k){
            graph.clear();
            int num_nodes = 2+k+2*inputLength;
            int source = 0;
            int sink = num_nodes-1;
            std::vector<int> s;
            for(int i = 1; i<k+1; i++)
                s.push_back(i);
            std::vector<int> r;
            std::vector<int> rprime;
            for(int i = 0;i<inputLength;i++){
                r.push_back(k+1+2*i);
                rprime.push_back(k+2+2*i);
            }
            
        }
        int computeMCFP(){
            return 0;
        }
        ~Mcfp(){}
};

#endif