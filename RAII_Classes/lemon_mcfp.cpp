//author: Stefan Caldararu

#ifndef MCFPnew
#define MCFPnew

#include <vector>
#include "../mspace.h"
#include<iostream>
#include<stack>
#include<algorithm>
#include <lemon/list_graph.h>
#include <lemon/cost_scaling.h>

using namespace lemon;

class Lemon_Mcfp
{
    private:
        ListDigraph g;
        int inputLength;
        ListDigraph::Node source;
        int num_nodes;
        ListDigraph::Node sink;
        std::vector<ListDigraph::Node> s;
        std::vector<ListDigraph::Node> sprime;
        std::vector<ListDigraph::Node> r;
        std::vector<ListDigraph::Node> rprime;
        int k;
    public:
        Lemon_Mcfp(){}
        //This constructor is for the WF alg, here we return which server to move, and also include a final configuration. This both sets the graph and runs the algorithm
        int setGraph(Mspace& metricSpace, std::vector<int> Sigma, int IL, int num_servers, std::vector<int> init_config, std::vector<int> final_config){
            g.clear();
            ListDigraph::ArcMap<int> cap(g);
            ListDigraph::ArcMap<int> cost(g);
            inputLength = IL;
            k = num_servers;
            num_nodes = 2+2*k+2*inputLength;
            source = g.addNode();
            sink = g.addNode();
            s.clear();
            r.clear();
            sprime.clear();
            rprime.clear();
            for(int i = 0;i<k;i++){
                ListDigraph::Node temp1 = g.addNode();
                ListDigraph::Node temp2 = g.addNode();
                s.push_back(temp1);
                sprime.push_back(temp2);
            }
            for(int i = 0;i<inputLength;i++){
                ListDigraph::Node temp1 = g.addNode();
                ListDigraph::Node temp2 = g.addNode();
                r.push_back(temp1);
                rprime.push_back(temp2);
            }
            
            //calc x.
            int x = 0;
            for(int i = 0;i<k;i++)
                x = x+metricSpace.getDistance(final_config[i], Sigma[inputLength-1]);
            if(x>1)
                x = x/(k-1);
            //TODO: dont forget to add all of the capacities, and make them all 1.
            std::vector<ListDigraph::Arc> arcs_to_sink;
            for(int i = 0;i<k;i++){
                ListDigraph::Arc a = g.addArc(source, s[i]);
                cost[a] = 0;
                for(int j = 0;j<r.size();j++){
                    ListDigraph::Arc a1 = g.addArc(s[i], r[j]);
                    cost[a1] = metricSpace.getDistance(init_config[i], Sigma[j]);
                }
                for(int j = 0;j<k;j++){
                    ListDigraph::Arc a1 = g.addArc(s[i], sprime[j]);
                    cost[a1] = metricSpace.getDistance(init_config[i], final_config[j]);
                }
                ListDigraph::Arc a1 = g.addArc(sprime[i], sink);

                cost[a1] = x-metricSpace.getDistance(final_config[i], Sigma[inputLength-1]);
                arcs_to_sink.push_back(a1);
            }

            for(int i = 0;i<inputLength;i++){
                ListDigraph::Arc a = g.addArc(r[i], rprime[i]);
                cost[a] = -1e6;
                for(int j = i+1;j<inputLength;j++){
                    ListDigraph::Arc a1 = g.addArc(rprime[i],r[j]);
                    cost[a1] = metricSpace.getDistance(Sigma[j], Sigma[i]);
                }
                if(i+1 != inputLength){
                    for(int j = 0;j<k;j++){
                        ListDigraph::Arc a1 = g.addArc(rprime[i], sprime[j]);
                        cost[a1] = metricSpace.getDistance(Sigma[i], final_config[j]);
                    }
                }
            }
            ListDigraph::Arc a1 = g.addArc(rprime[inputLength-1], sink);
            cost[a1] = 0;

            for (ListDigraph::ArcIt a(g); a != INVALID; ++a)
                cap[a] = 1;


            CostScaling<ListDigraph> cs(g);
            cs.upperMap(cap);
            cs.costMap(cost);
            cs.stSupply(source, sink, k);
            cs.run();
            ListDigraph::ArcMap<int> flow(g);
            cs.flowMap(flow);
            //Now the flow through the graph is k, one of the sprime shouldn't be used. find which one it is, and return that int(server ID.)
            for(int i = 0;i<k;i++)
                if(flow[arcs_to_sink[i]] != 1)
                    return i;
            //We are returning the server that we want to move (its ID).
            return -1;
        }

        int setGraph(Mspace & metricSpace, std::vector<int> Sigma, int IL, int num_servers, std::vector<int> init_config){
            g.clear();
            ListDigraph::ArcMap<int> cap(g);
            ListDigraph::ArcMap<int> cost(g);
            inputLength = IL;
            k = num_servers;
            num_nodes = 2+2*k+2*inputLength;
            source = g.addNode();
            sink = g.addNode();
            s.clear();
            r.clear();
            rprime.clear();
            for(int i = 0;i<k;i++){
                ListDigraph::Node temp1 = g.addNode();
                s.push_back(temp1);
            }
            for(int i = 0;i<inputLength;i++){
                ListDigraph::Node temp1 = g.addNode();
                ListDigraph::Node temp2 = g.addNode();
                r.push_back(temp1);
                rprime.push_back(temp2);
            }
            
            //Now, we need to populate the graph as     
            //Reference: in notes, 0404017, page 179/180. "NEW RESULTS ON SERVER PROBLEMS, M. CHROBAK"
            for(int i = 0;i<k;i++){
                ListDigraph::Arc a = g.addArc(source, s[i]);
                cost[a] = 0;
                ListDigraph::Arc a1 = g.addArc(s[i], sink);
                cost[a1] = 0;
                for(int j = 0;j<inputLength;j++){
                    ListDigraph::Arc a2 = g.addArc(s[i], r[j]);
                    cost[a2] = metricSpace.getDistance(init_config[i], Sigma[j]);
                }
            }
            for(int i = 0;i<inputLength;i++){
                ListDigraph::Arc a = g.addArc(rprime[i], sink);
                cost[a] = 0;
                for(int j = i+1;j<inputLength;j++){
                    ListDigraph::Arc a1 = g.addArc(rprime[i], r[j]);
                    cost[a1] = metricSpace.getDistance(Sigma[i], Sigma[j]);
                }
                ListDigraph::Arc a1 = g.addArc(r[i], rprime[i]);
                cost[a1] = -1e6;
            }
            for (ListDigraph::ArcIt a(g); a != INVALID; ++a)
                cap[a] = 1;
            
            CostScaling<ListDigraph> cs(g);
        cs.upperMap(cap);
        cs.costMap(cost);
        cs.stSupply(source, sink, k);

        cs.run();
        ListDigraph::ArcMap<int> flow(g);
        cs.flowMap(flow);
        int min_cost = cs.totalCost()+1e6*inputLength;

        return min_cost;

            

        }

        ~Lemon_Mcfp(){}
};

#endif