//author: Stefan Caldararu

#include <cstddef>
#include <vector>

class Mspace
{
    private:
        std::size_t m;
    public:
        //The metric space, an adjacency matrix of distances between nodes
        //FIXME: there is a bug because I want this to be a vector, not a pointer.
        int ** graph;
        //The default constructor, without a size.
        Mspace();
        //The constructor
        Mspace(std::size_t size);
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