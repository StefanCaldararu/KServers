#include <iostream>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

int main() {
    // Define the graph using an adjacency_list
    typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_weight_t, int, property<edge_cost_t, int>>> Graph;

    // Define the edge properties
    typedef property_map<Graph, edge_weight_t>::type WeightMap;
    typedef property_map<Graph, edge_cost_t>::type CostMap;

    // Define the vertices and edges of the graph
    const int num_vertices = 4;
    enum { S, A, B, T };
    Graph g(num_vertices);
    WeightMap weight = get(edge_weight, g);
    CostMap cost = get(edge_cost, g);

    // Add the edges to the graph
    add_edge(S, A, {2, 3}, g); // Edge from S to A with weight 2 and cost 3
    add_edge(S, B, {1, 4}, g); // Edge from S to B with weight 1 and cost 4
    add_edge(A, B, {1, 2}, g); // Edge from A to B with weight 1 and cost 2
    add_edge(A, T, {1, 5}, g); // Edge from A to T with weight 1 and cost 5
    add_edge(B, T, {2, 1}, g); // Edge from B to T with weight 2 and cost 1

    // Print the edges of the graph with their weights and costs
    graph_traits<Graph>::edge_iterator e, e_end;
    for (tie(e, e_end) = edges(g); e != e_end; ++e) {
        std::cout << "Edge (" << source(*e, g) << ", " << target(*e, g) << ") with weight " << weight[*e] << " and cost " << cost[*e] << std::endl;
    }

    return 0;
}