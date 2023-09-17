#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "RAII_Classes/getInput.cpp"
#include "ALGS/doubleCoverageAlg.h"
#include "ALGS/lazyDoubleCoverageAlg.h"

int parseInput(char* inputFile, char*outputFile, int argc, char** argv)
{
    for(int i = 0; i<argc; i++){
        if(strcmp(argv[i], "-input") == 0 || strcmp(argv[i], "-i") == 0){
            if(i+1<=argc)
                strcpy(inputFile, argv[i+1]);
            else
                return 1;
        }
        if(strcmp(argv[i], "-output") == 0 || strcmp(argv[i], "-o") == 0){
            if(i+1<=argc)
                strcpy(outputFile, argv[i+1]);
            else
                return 1;
        }
    }
    return 0;
}

int main(int argc, char ** argv)
{
    char* inputFile = new char[200];
    char* outputFile = new char[200];

    if(parseInput(inputFile, outputFile, argc, argv) == 1)
        return 1;

    GetInput reader(inputFile);
    //The algs we want to run
    reader.getLine();
    //The number of mspaces(1)
    reader.getLine();
    Mspace mspace;
    //Number of nodes for this input, size of the mspace
    reader.getLine();
    int size = std::stoi(reader.line);
    mspace.setSize(size);
    std::string word;
    //populate the mspace!
    for(int j = 0; j<size; j++){
        reader.getLine();
        std::stringstream str(reader.line);
        int k = 0;
        while(getline(str, word, ',')){
            mspace.setDistance(j, k, stoi(word));
            k++;
        }
    }
    //The number of inputs
    reader.getLine();
    int num_inputs = std::stoi(reader.line);

    //and number of servers...
    reader.getLine();
    int num_servers = std::stoi(reader.line);

    //Set the server locations
    std::vector<int> server_locations;
    server_locations.reserve(num_servers);
    for(int j = 0;j<num_servers;j++)
        server_locations.push_back(j);
    //the input
    reader.getLine();
    std::vector<int> input;
    std::stringstream str(reader.line);
    while(getline(str, word, ',') && input.size() <=num_inputs)
        input.push_back(stoi(word));

    DoubleCoverageAlg dalg;
    LazyDoubleCoverageAlg ldalg;

    dalg.setGraph(mspace);
    dalg.setServers(num_servers, server_locations);
    //std::cout << "NUM SERVERS: " << num_servers << std::endl;
    std::cout << dalg.runAlg(input, num_inputs) << std::endl;

    ldalg.setGraph(mspace);
    ldalg.setServers(num_servers, server_locations);
    std::cout << ldalg.runAlg(input, num_inputs) << std::endl;
    
    return 0;
}