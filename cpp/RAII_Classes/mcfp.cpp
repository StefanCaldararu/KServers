//author: Stefan Caldararu

#ifndef MCFP
#define MCFP

#include <vector>
#include "../mspace.h"
#include<iostream>
#include<stack>
#include<algorithm>
#include <chrono>


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
        Mspace metricSpace;
        std::vector<int> inputs;
        std::vector<int> in_conf;
        std::vector<int> fin_conf;
        int inputLength;
        int source;
        int num_nodes;
        int sink;
        std::vector<int> s;
        std::vector<int> sprime;
        std::vector<int> r;
        std::vector<int> rprime;
        int k;
        int x;
    public:
        Mcfp(){}
        //Set the graph for the WFA.
        void setGraph_WFA(Mspace ms, std::vector<int> Sigma, int IL, int num_servers, std::vector<int> init_config, std::vector<int> final_config){
            inputs = Sigma;
            metricSpace = ms;
            in_conf = init_config;
            fin_conf = final_config;
            graph.clear();
            inputLength = IL;
            k = num_servers;
            num_nodes = 2+2*k+2*inputLength;
            source = 0;
            sink = num_nodes-1;
            s.clear();
            r.clear();
            sprime.clear();
            rprime.clear();
            for(int i = 1;i<k+1;i++){
                s.push_back(2*i-1);
                sprime.push_back(2*i);
            }
            for(int i = 0;i<inputLength;i++){
                r.push_back(2*k+1+2*i);
                rprime.push_back(2*k+2+2*i);
            }
            graph.reserve(num_nodes);
            for(int i = 0;i<num_nodes;i++){
                std::vector<edge> temp;
                temp.reserve(num_nodes);
                for(int j = 0;j<num_nodes;j++){
                    temp.push_back(edge(i,j,0,0));
                }
                graph.push_back(temp);
            }
            //calc x.
            x = 0;
            for(int i = 0;i<k;i++)
                x = x+metricSpace.getDistance(final_config[i], Sigma[inputLength-1]);
            if(x>1)
                x = x/(k-1);


            for(int i = 0;i<k;i++){
                graph[source][s[i]] = edge(source, s[i], 0, 1);
                for(int j = 0;j<r.size();j++)
                    graph[s[i]][r[j]] = edge(s[i], r[j], metricSpace.getDistance(init_config[i], Sigma[j]), 1);
                for(int j = 0;j<k;j++){
                    graph[s[i]][sprime[j]] = edge(s[i], sprime[j], metricSpace.getDistance(init_config[i], final_config[j]), 1);
                }
                graph[sprime[i]][sink] = edge(sprime[i], sink, x-metricSpace.getDistance(final_config[i],Sigma[inputLength-1]),1);
            }
            for(int i = 0;i<inputLength;i++){
                graph[r[i]][rprime[i]] = edge(r[i], rprime[i], -1e6, 1);
                for(int j = i+1; j<inputLength;j++)
                    graph[rprime[i]][r[j]] = edge(rprime[i], r[j], metricSpace.getDistance(Sigma[j], Sigma[i]), 1);
                //if(i+1 != inputLength)    
                for(int j = 0;j<k;j++)
                    graph[rprime[i]][sprime[j]] = edge (rprime[i], sprime[j], metricSpace.getDistance(Sigma[i], final_config[j]), 1);
            }
            graph[rprime[inputLength-1]][sink] = edge(rprime[inputLength-1], sink, 0, 1);
        }
        int compute(){
            std::vector<std::vector<edge> > temp = graph;
            int cost = computeMCFP();
            for(int i = 0;i<k;i++){
                if(graph[sprime[i]][sink].flow == 1){
                    graph = temp;
                    return i;
                }
            }
            graph = temp;
            return -1;
        }

        void moveServer(int id, int loc){
            if(k!=1)
                x = x*(k-1);
            x = x-metricSpace.getDistance(fin_conf[id], inputs[inputLength-1])+metricSpace.getDistance(loc, inputs[inputLength-1]);
            fin_conf[id] = loc;
            if(k!=1)
                x = x/(k-1);
            graph[sprime[id]][sink] = edge(sprime[id],sink,x-metricSpace.getDistance(loc, sink),1);
            for(int i = 0;i<inputLength;i++)
                graph[rprime[i]][sprime[id]] = edge(rprime[i], sprime[id], metricSpace.getDistance(inputs[i], loc), 1);
            for(int i= 0;i<k;i++)
                graph[s[i]][sprime[id]] = edge(s[i], sprime[id], metricSpace.getDistance(in_conf[i], loc), 1);
        }

        void add_input(int request){
            graph[rprime[inputLength-1]][sink] = edge(rprime[inputLength-1], sink, 0,0);
            //now add all of the appropriate numbers...
            inputLength++;
            inputs.push_back(request);
            r.push_back(num_nodes);
            for(int i = 0;i<num_nodes;i++){
                graph[i].push_back(edge(i, num_nodes, 0,0));
            }
            num_nodes++;
            std::vector<edge> temp;
            for(int i = 0;i<num_nodes;i++){
                temp.push_back(edge(num_nodes-1, i, 0,0));
            }
            graph.push_back(temp);
            //Now the graph is updated, and includes the new request. we must now update all of the costs and flows...
            for(int i = 0;i<k;i++){
                graph[s[i]][r[inputLength-1]] = edge(s[i], r[inputLength-1], metricSpace.getDistance(in_conf[i], request), 1);
            }
            for(int i = 0;i<inputLength-1;i++){
                graph[rprime[i]][r[inputLength-1]] = edge(rprime[i], r[inputLength-1], metricSpace.getDistance(inputs[i], request), 1);
            }

            //Now need to do everything for rprime...
            rprime.push_back(num_nodes);
            for(int i = 0;i<num_nodes;i++){
                graph[i].push_back(edge(i, num_nodes, 0, 0));
            }
            num_nodes ++;
            temp.clear();
            for(int i = 0;i<num_nodes;i++){
                temp.push_back(edge(num_nodes-1, i, 0,0));
            }
            graph.push_back(temp);
            //Now the graph is updated, and includes the new rprime. update costs and flows...
            graph[r[inputLength-1]][rprime[inputLength-1]] = edge(r[inputLength-1],rprime[inputLength-1], -1e6, 1);
            graph[rprime[inputLength-1]][sink]= edge(rprime[inputLength-1], sink, 0, 1);
            for(int i = 0;i<k;i++){
                graph[rprime[inputLength-1]][sprime[i]] = edge(rprime[inputLength-1],sprime[i], metricSpace.getDistance(request, fin_conf[i]), 1);
            }
            //Need to recalculate x...
            x = 0;
            for(int i = 0;i<k;i++)
                x = x+metricSpace.getDistance(fin_conf[i], inputs[inputLength-1]);
            if(x>1)
                x = x/(k-1);
            for(int i = 0;i<k;i++)
                graph[sprime[i]][sink] = edge(sprime[i], sink, x-metricSpace.getDistance(fin_conf[i],inputs[inputLength-1]),1);
            
        }




        //This constructor is for the WF alg, here we return which server to move, and also include a final configuration. This both sets the graph and runs the algorithm
        int setGraph(Mspace& metricSpace, std::vector<int> Sigma, int IL, int num_servers, std::vector<int> init_config, std::vector<int> final_config){
            graph.clear();
            inputLength = IL;
            k = num_servers;
            num_nodes = 2+2*k+2*inputLength;
            source = 0;
            sink = num_nodes-1;
            s.clear();
            r.clear();
            sprime.clear();
            rprime.clear();
            for(int i = 1;i<k+1;i++){
                s.push_back(2*i-1);
                sprime.push_back(2*i);
            }
            for(int i = 0;i<inputLength;i++){
                r.push_back(2*k+1+2*i);
                rprime.push_back(2*k+2+2*i);
            }
            graph.reserve(num_nodes);
            for(int i = 0;i<num_nodes;i++){
                std::vector<edge> temp;
                temp.reserve(num_nodes);
                for(int j = 0;j<num_nodes;j++){
                    temp.push_back(edge(i,j,0,0));
                }
                graph.push_back(temp);
            }
            //calc x.
            int x = 0;
            for(int i = 0;i<k;i++)
                x = x+metricSpace.getDistance(final_config[i], Sigma[inputLength-1]);
            if(x>1)
                x = x/(k-1);


            for(int i = 0;i<k;i++){
                graph[source][s[i]] = edge(source, s[i], 0, 1);
                for(int j = 0;j<r.size();j++)
                    graph[s[i]][r[j]] = edge(s[i], r[j], metricSpace.getDistance(init_config[i], Sigma[j]), 1);
                for(int j = 0;j<k;j++){
                    graph[s[i]][sprime[j]] = edge(s[i], sprime[j], metricSpace.getDistance(init_config[i], final_config[j]), 1);
                }
                graph[sprime[i]][sink] = edge(sprime[i], sink, x-metricSpace.getDistance(final_config[i],Sigma[inputLength-1]),1);
            }
            for(int i = 0;i<inputLength;i++){
                graph[r[i]][rprime[i]] = edge(r[i], rprime[i], -1e6, 1);
                for(int j = i+1; j<inputLength;j++)
                    graph[rprime[i]][r[j]] = edge(rprime[i], r[j], metricSpace.getDistance(Sigma[j], Sigma[i]), 1);
                if(i+1 != inputLength)    
                    for(int j = 0;j<k;j++)
                        graph[rprime[i]][sprime[j]] = edge (rprime[i], sprime[j], metricSpace.getDistance(Sigma[i], final_config[j]), 1);
            }
            graph[rprime[inputLength-1]][sink] = edge(rprime[inputLength-1], sink, 0, 1);
            //Now the graph should be set up correctly.
            //Now, we want to compute the graph. We don't care about what the cost is, but we want to figure out afterwards which final_node wasn't used.
            int cost = computeMCFP();
            //Now the flow through the graph is k, one of the sprime shouldn't be used. find which one it is, and return that int(server ID.)
            for(int i = 0;i<k;i++)
                if(graph[sprime[i]][sink].flow == 1)
                    return i;
            //We are returning the server that we want to move (its ID).
            return -1;
        }

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
                std::vector<edge> path = findMinCostPath(source, sink);

                cost += reversePath(path);
            }
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