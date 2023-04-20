#include <lemon/list_graph.h>
#include <lemon/cost_scaling.h>
#include "RAII_Classes/getInput.cpp"
#include "mspace.h"
#include "ALGS/alg.h"
#include "ALGS/doubleCoverageAlg.h"
#include "ALGS/randomAlg.h"
#include "ALGS/greedyAlg.h"
#include "ALGS/optAlg.h"
#include "ALGS/WFAlg.h"
#include "RAII_Classes/lemon_mcfp.cpp"
using namespace lemon;

int main()
{   
    Mspace metricSpace;
    metricSpace.setSize(2);
    metricSpace.setDistance(0,0,0);
    metricSpace.setDistance(0,1,1);
    metricSpace.setDistance(1,1,0);
    metricSpace.setDistance(1,0,1);
    std::vector<int> Sigma;
    Sigma.push_back(0);
    Sigma.push_back(1);
    int IL = 2;
    int k = 1;
    std::vector<int> config;
    config.push_back(1);
    std::vector <Alg*> runningAlgs;
    runningAlgs.push_back(new GreedyAlg());    
    runningAlgs.push_back(new OptAlg());
    // runningAlgs.push_back(new WFAlg);
    // runningAlgs.push_back(new DoubleCoverageAlg);
    // runningAlgs.push_back(new RandomAlg);

    for(auto alg:runningAlgs){
        alg->setGraph(metricSpace);
        alg->setServers(k,config);
        int cost = alg->runAlg(Sigma, IL);
    }
    for(auto alg:runningAlgs)
        delete alg;
    return 0;
    // ListDigraph g;
    // ListDigraph::ArcMap<int> cap(g); // Arc capacity map
    // ListDigraph::ArcMap<int> cost(g); // Arc cost map
    // ListDigraph::Node s = g.addNode();
    // ListDigraph::Node t = g.addNode();

    // ListDigraph::Node s0 = g.addNode();
    // ListDigraph::Node s1 = g.addNode();
    // ListDigraph::Node r1 = g.addNode();
    // ListDigraph::Node r2 = g.addNode();
    // ListDigraph::Node r3 = g.addNode();
    // ListDigraph::Node r1p = g.addNode();
    // ListDigraph::Node r2p = g.addNode();
    // ListDigraph::Node r3p = g.addNode();
    // ListDigraph::Arc a1 = g.addArc(s, s0);
    // cost[a1] = 0;
    // ListDigraph::Arc a2 = g.addArc(s, s1);
    // cost[a2] = 0;

    // ListDigraph::Arc s0_1 = g.addArc(s0, r1);
    // cost[s0_1] = 2;
    // ListDigraph::Arc s0_2 = g.addArc(s0, r2);
    // cost[s0_2] = 0;
    // ListDigraph::Arc s0_3 = g.addArc(s0, r3);
    // cost[s0_3] = 2;
    // ListDigraph::Arc s0_4 = g.addArc(s0, t);
    // cost[s0_3] = 0;
    // ListDigraph::Arc s1_1 = g.addArc(s1, r1);
    // cost[s1_1] = 4;
    // ListDigraph::Arc s1_2 = g.addArc(s1, r2);
    // cost[s1_2] = 3;
    // ListDigraph::Arc s1_3 = g.addArc(s1, r3);
    // cost[s1_3] = 4;
    // ListDigraph::Arc s1_4 = g.addArc(s1, t);
    // cost[s1_4] = 0;

    // ListDigraph::Arc k1 = g.addArc(r1, r1p);
    // cost[k1] = -1e6;
    // ListDigraph::Arc k2 = g.addArc(r2, r2p);
    // cost[k2] = -1e6;
    // ListDigraph::Arc k3 = g.addArc(r3, r3p);
    // cost[k3] = -1e6;

    // ListDigraph::Arc r12 = g.addArc(r1p, r2);
    // cost[r12] = 2;
    // ListDigraph::Arc r13 = g.addArc(r1p, r3);
    // cost[r13] = 0;
    // ListDigraph::Arc r23 = g.addArc(r2p, r3);
    // cost[r23] = 2;
    // ListDigraph::Arc r1pp = g.addArc(r1p, t);
    // cost[r1pp] = 0;
    // ListDigraph::Arc r2pp = g.addArc(r2p, t);
    // cost[r2pp] = 0;
    // ListDigraph::Arc r3pp = g.addArc(r3p, t);
    // cost[r3pp] = 0;

    // for (ListDigraph::ArcIt a(g); a != INVALID; ++a)
    //     cap[a] = 1;

    // CostScaling<ListDigraph> cs(g);
    // cs.upperMap(cap);
    // cs.costMap(cost);
    // cs.stSupply(s, t, 2);

    // cs.run();
    // ListDigraph::ArcMap<int> flow(g);
    // cs.flowMap(flow);
    // int max_flow = 0;
    // for (ListDigraph::OutArcIt a(g, s); a != INVALID; ++a) {
    //     max_flow += flow[a];
    // }
    // int min_cost = cs.totalCost();

    // std::cout << "Minimum cost maximum flow: " << min_cost << std::endl;
    // std::cout <<"Flow at arc r3pp: " << flow[r3pp]<<std::endl;
    // std::cout << "Maximum flow: " << max_flow << std::endl;

    // return 0;
}