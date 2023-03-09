//author: Stefan Caldararu

#include <cstddef>

//template<std::size_t ArraySize>
class Mspace
{
    private:
        std::size_t m;
    public:
        //The metric space, an adjacency matrix of distances between nodes
        int ** graph;
        //The constructor
        Mspace(int ArraySize);
        //returns the distance between a source and destination
        int getDistance(int source, int destination);
        //sets the distance between a source and destination
        void setDistance(int source, int destination, int distance);
        //resets the graph to have distances of -1 between all points.
        void clear();
        //The destructor
        ~Mspace();
};
