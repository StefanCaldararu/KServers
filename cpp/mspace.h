//author: Stefan Caldararu
#ifndef MSPACE
#define MSPACE
#include <cstddef>
#include <vector>

class Mspace
{
    private:
        std::size_t m;
    public:
        //The metric space, an adjacency matrix of distances between nodes
        std::vector<std::vector<int> > graph;
        //The default constructor, without a size.
        Mspace();
        //The constructor
        // Mspace(std::size_t size);
        //The copy constructor
        // Mspace(Mspace& t);
        //Set the size of this metric space
        void setSize(std::size_t size);
        //Returns the number of points in this metric space
        int getSize();
        //returns the distance between a source and destination
        int getDistance(int source, int destination);
        //sets the distance between a source and destination
        void setDistance(int source, int destination, int distance);
        //resets the graph to have distances of -1 between all points.
        void clear();
        //The destructor
        ~Mspace();
};

#endif