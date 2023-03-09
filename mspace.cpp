//author: Stefan Caldararu

#include "mspace.h"

Mspace::Mspace(){
    m = 0;
}

Mspace::Mspace(std::size_t size)
{
    m = size;
    graph = new int *[m];
    for(int i = 0; i<m; i++){
        graph[i] = new int[m];
        for (int j = 0;j<m;j++)
            graph[i][j] = -1;
    }
}

void Mspace::setSize(std::size_t size)
{
    if(m){
        for(int i = 0;i<m;i++)
            delete [] graph[i];
        delete [] graph;
    }
    m = size;
    graph = new int *[m];
    for(int i = 0; i<m; i++){
        graph[i] = new int[m];
        for (int j = 0;j<m;j++)
            graph[i][j] = -1;
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

void Mspace::clear()
{
    for(int i = 0; i<m; i++)
        for(int j = 0;j<m;j++)
            graph[i][j] = -1;
}

Mspace::~Mspace()
{
    for(int i = 0; i<m; i++)
        delete [] graph[i];
    delete [] graph;
}