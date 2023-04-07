//author: Stefan Caldararu

#ifndef MCFP
#define MCFP

#include <vector>
#include "../mspace.h"
#include<iostream>
#include<stack>
#include<algorithm>


struct edge{
    int src;
    int dest;
    int cost;
    int flow;
    edge(int s, int d, int c, int f){
        src = s;
        dest = d;
        cost = c;
        flow = f;
    }
};

class Mcfp
{
    private:
        std::vector<std::vector<edge> > graph;
        int inputLength;
        int source;
        int num_nodes;
        int sink;
        std::vector<int> s;
        std::vector<int> r;
        std::vector<int> rprime;
        int k;
    public:
        Mcfp(){}
        void setGraph(Mspace & metricSpace, std::vector<int> Sigma, int IL, int num_servers, std::vector<int> init_config){
            graph.clear();
            inputLength = IL;
            k = num_servers;
            num_nodes = 2+k+2*inputLength;
            source = 0;
            sink = num_nodes-1;
            s.clear();
            r.clear();
            rprime.clear();
            for(int i = 1; i<k+1; i++)
                s.push_back(i);
            for(int i = 0;i<inputLength;i++){
                r.push_back(k+1+2*i);
                rprime.push_back(k+2+2*i);
            }
            graph.reserve(num_nodes);
            //Now want to make sure that graph is an adjacency matrix, with source, dest as index.
            for(int i = 0;i<num_nodes;i++){
                std::vector<edge> temp;
                temp.reserve(num_nodes);
                // graph.push_back(temp);
                // graph[i].reserve(num_nodes);
                for(int j = 0;j<num_nodes;j++){
                    temp.push_back(edge(i,j,0,0));
                    std::cout << "THIS RAN FOR I: " << std::to_string(i) << " AND J: " << std::to_string(j) << "\n";
                }
                graph.push_back(temp);
            }
            //Now, we need to populate the graph as     
            //Reference: in notes, 0404017, page 179/180. "NEW RESULTS ON SERVER PROBLEMS, M. CHROBAK"
            for(int i = 0;i<k;i++){
                graph[source][s[i]] = edge(source, s[i],0,1);
                graph[s[i]][sink] = edge(s[i], sink, 0,1);
                for(int j = 0;j<inputLength;j++)
                    graph[s[i]][r[j]] = edge(s[i], r[j], metricSpace.getDistance(init_config[i],Sigma[j]),1);
            }
            for(int i = 0;i<inputLength;i++){
                graph[rprime[i]][sink] = edge(rprime[i], sink, 0,1);
                for(int j = i+1;j<inputLength;j++)
                    graph[rprime[i]][r[j]] = edge(rprime[i], r[j], metricSpace.getDistance(Sigma[i], Sigma[j]), 1);
                //FIXME: make this not hardcoded...
                graph[r[i]][rprime[i]] = edge(r[i], rprime[i], -1e6,1);
            }
            

        }
        int computeMCFP(){
            int cost = 1e6*inputLength;
            while(pathexists()){
                std::vector<edge> path = minCostBF(source, sink);

                cost += reversePath(path);
            }
            // cost = cost+1e6*inputLength;
            // //Now we have the residual graph. we want to do the same thing from the sink.
            // int cost = 0;
            // for(int i = 0;i<k;i++){
            //     std::vector <edge> path = minCostBF(sink, s[i]);
            //     for(int j = 0;j<path.size();j++)
            //         if(path[j].cost!=-1e6 && path[j].cost!=1e6)
            //             cost = cost+path[j].cost;
            // }
            return cost;

        }
        //check if a path exists from src to dest.
        bool pathexists(){
            for(int i = 0;i<k;i++)
                if(graph[source][s[i]].flow == 1)
                    return true;
            return false;
        }

        std::vector<edge> minCostBF(int src, int dest){
            std::vector< std::vector<int> > paths;
            std::vector<int> curPath = {src};
            generatePaths(src, dest, curPath, paths);

            int locShortest = 0;
            int cost = computePathLength(paths[0]);
            for(int i = 1; i<paths.size();i++){
                int thiscost = computePathLength(paths[i]);
                if(thiscost<cost){
                    locShortest = i;
                    cost = thiscost;
                }
            }
            std::vector<int> bestpath = paths[locShortest];
            std::vector<edge> ret;
            int crnt = bestpath[0];
            for(int i = 1;i<bestpath.size();i++){
                ret.push_back(graph[crnt][bestpath[i]]);
                crnt = bestpath[i];
            }
            return ret;

        }
        int computePathLength(std::vector<int> path){
            int loc = path[0];
            int cost = 0;
            for(int i = 1; i<path.size();i++){
                cost = cost+graph[loc][path[i]].cost;
                loc = path[i];
            }
            return cost;
        }

        void generatePaths(int src, int dest, std::vector<int>& curPath, std::vector<std::vector<int> >& paths){
            if(src == dest){
                paths.push_back(curPath);
            }
            else{
                for(int i = 0;i<num_nodes;i++){
                    if(graph[src][i].flow == 1 && !std::count(curPath.begin(), curPath.end(), i)){
                        curPath.push_back(i);
                        generatePaths(i, dest, curPath, paths);
                        curPath.pop_back();
                    }
                }
            }
        }
        //TODO: probably want to check this works...lol
        std::vector<edge> findMinCostPath(int src, int dest){
            std::vector<int> dist(num_nodes, 1e6);
            std::vector<int> pred(num_nodes, -1);
            dist[src] = 0;
            for(int i = 0;i<num_nodes-1;i++){
                for(int u = 0;u<num_nodes;u++){
                    for(int v = 0;v<num_nodes;v++){
                        if(graph[u][v].flow != 0 && (dist[u] != 1e6 && dist[u]+graph[u][v].cost < dist[v])){
                            dist[v] = dist[u]+graph[u][v].cost;
                            pred[v] = u;
                        }
                    }
                }
            }
            //std::cout << "Shortest path from "<< src << " to " <<dest << ": ";
            std::stack<int> path;
            int node = dest;
            while(node!=src){
                path.push(node);
                node = pred[node];
            }
            path.push(src);
            // while(!path.empty()){
            //     std::cout<< path.top()<< " ";
            //     path.pop;
            // }
            // std::cout << std::endl;
            std::vector<edge> ret;
            int crnt_node = path.top();
            path.pop();
            while(!path.empty()){
                ret.push_back(graph[crnt_node][path.top()]);
                crnt_node = path.top();
                path.pop();
            }
            std::cout << "Shortest distance from source to sink: " << dist[dest] << std::endl;

            return ret;

        }

        int reversePath(std::vector<edge> path){
            int cost = 0;
            for(int i = 0;i<path.size();i++){
                graph[path[i].dest][path[i].src].flow = 1;
                cost += graph[path[i].src][path[i].dest].cost;
                graph[path[i].dest][path[i].src].cost = -graph[path[i].src][path[i].dest].cost;
                graph[path[i].src][path[i].dest].cost = 0;
                graph[path[i].src][path[i].dest].flow = 0; 
            }
            return cost;
        }

        ~Mcfp(){}
};

#endif