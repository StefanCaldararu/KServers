//author: Stefan Caldararu

#include "mspace.h"

Mspace::Mspace(){
    m = 0;
}

// Mspace::Mspace(std::size_t size)
// {
//     m = size;
//     graph.reserve(size);
//     for(int i = 0; i<m; i++){
//         graph[i].reserve(size);
//         for (int j = 0;j<m;j++)
//             graph[i][j] = -1;
//     }
// }

// Mspace::Mspace(Mspace& t){
//     setSize(t.m);
//     for(int i = 0; i< m; i++)
//         for(int j = 0;j<m; j++)
//             setDistance(i, j, t.getDistance(i,j));
// }

void Mspace::setSize(std::size_t size)
{
    graph.clear();
    // if(m){
    //     for(int i = 0;i<m;i++)
    //         delete [] graph[i];
    //     delete [] graph;
    // }
    m = size;
    graph.reserve(m);
    //graph = new int *[m];
    for(int i = 0; i<m; i++){
        graph.push_back(std::vector<int> ());
        graph[i].reserve(m);
        //graph[i] = new int[m];
        for (int j = 0;j<m;j++)
            graph[i].push_back(-1);
    }   
}

int Mspace::getSize()
{
    return m;
}

int Mspace::getDistance(int source, int destination)
{
    return graph[source][destination];
}

void Mspace::setDistance(int source, int destination, int distance)
{
    graph[source][destination] = distance;
}

std::vector<int> Mspace::findTreePathDFS(int source, int destination)
{
    std::vector<bool> visited(m, false);
    std::stack<int> s;
    std::vector<int> path = std::vector<int>(m, -1);
    s.push(source);
    visited[source] = true;

    while(!s.empty()){
        int current = s.top();
        s.pop();
        for(int i = 0; i<m; i++){
            if(graph[current][i] != -1 && !visited[i]){
                s.push(i);
                visited[i] = true;
                path[i] = current;
                if(i == destination){
                    int node = destination;
                    while(node != source){
                        path.push_back(node);
                        node = path[node];
                    }
                }
            }
        }
    }
    //add the source to the path
    path.push_back(source);
    //reverse the order so that the source is at the beginning
    std::reverse(path.begin(), path.end());
    //remove the -1's as well as the extra elements from the vector
    return path;
}

void Mspace::clear()
{
    for(int i = 0; i<m; i++)
        for(int j = 0;j<m;j++)
            graph[i][j] = -1;
}

Mspace::~Mspace()
{
    
}