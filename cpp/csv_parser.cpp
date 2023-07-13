//autor: Stefan Caldararu
//We want to be able to read directly from csv files, and then output ot csv files.
//want to take an input parameter, and an output parameter.
//options will be passed to this program, to denote:
//What algos we want to run
//What output level we want
//input and output file must be passed in a similar manner
//Input will be organized as follows:
//first line is number of mspaces.
//second line is this mspace
//third line is number of inputs for this metric space
//following line is each input, until we reach the next mspace, and follows as above.
#include "ALGS/WFAlg.h"
#include "ALGS/randomAlg.h"
#include "ALGS/optAlg.h"
#include "ALGS/greedyAlg.h"
#include "ALGS/doubleCoverageAlg.h"
#include "ALGS/KCentersAlg.h"

#include <iostream>
#include <string.h> 
#include <fstream>
#include <vector>
#include <sstream>
#include "RAII_Classes/getInput.cpp"
#include "RAII_Classes/writeOutput.cpp"


const int NUM_ALGS = 6;
//This takes in the argv and parses it for the main function. 
//gives pointer to input file, output file, and there is an array of which algorithms to run.
//TODO: probably want to create object for this.
//TODO: move the -algorithms option into the file.
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
//Returns the number of mspaces, or if doesn't work returns -1.
//TODO: create object for this whole function?
int getInput(char* inputFile,std::vector<int>& algsToRun, std::vector <std::vector<std::vector<int> > >& inputs,std::vector<std::vector<int> >& input_lengths, std::vector<int >& num_servers, std::vector<int>& num_inputs, std::vector <Mspace>& spaces)
{
    GetInput reader(inputFile);
    // reader.openFile(inputFile);
    //now first line is which algs to run. algs are sorted in the following order:
    //FIXME: whenever an alg is added, add it to this...
    reader.getLine();
    std::stringstream str(reader.line);
    std::string word;
    algsToRun.reserve(NUM_ALGS);
    while(getline(str, word, ','))
        algsToRun.push_back(stoi(word));

    //Now we have the algs we want to run
    //So, we want to get the number of mspaces.
    reader.getLine();
    int num_mspaces = std::stoi(reader.line);
    spaces.reserve(num_mspaces);
    //Now we have the number of mspaces.
    num_inputs.reserve(num_mspaces);
    inputs.reserve(num_mspaces);
    num_servers.reserve(num_mspaces);
    for(int i = 0; i<num_mspaces; i++){
        //Number of nodes for this input, size of the mspace
        reader.getLine();
        int size = std::stoi(reader.line);
        spaces.push_back(Mspace());
        spaces[i].setSize(size);
        //populate the mspace!
        for(int j = 0; j<size; j++){
            reader.getLine();
            std::stringstream str(reader.line);
            int k = 0;
            while(getline(str, word, ',')){
                spaces[i].setDistance(j, k, stoi(word));
                k++;
            }
            
        }
        //now the mspace is filled in correctly. we now need to make sure that 
        //the inputs all get filled in.
        //Number of inputs for this mspace
        reader.getLine();
        int NI = std::stoi(reader.line);
        num_inputs.push_back(NI);
        //Now we want to get the number of servers for this mspace. all inputs will have the same number of servers.
        reader.getLine();
        num_servers.push_back(std::stoi(reader.line));
        inputs.push_back(std::vector<std::vector<int> >());
        std::vector<int> input_length;
        for(int j = 0; j<NI; j++){
            std::vector<int> input;
            reader.getLine();
            std::string word;
            std::stringstream str(reader.line);
            while(getline(str, word, ','))
                input.push_back(stoi(word));
            inputs[i].push_back(input);
            input_length.push_back(input.size());
        }
        input_lengths.push_back(input_length);
    }
    return num_mspaces;

}

void printOutput(char* outputFile, std::vector<std::vector<int> >& costs, std::vector<int>& numInputs, std::vector<Mspace>& metricSpaces, std::vector<std::vector<std::vector<int> > >& inputs, std::vector<int> num_servers){
    WriteOutput writer(outputFile);
    int counter = 0;
    for(int i = 0;i<metricSpaces.size();i++){
        writer.writeLine("numserver: "+ std::to_string(num_servers[i]));
        for(int j = 0;j<metricSpaces[i].getSize();j++){
            writer.writeLine(metricSpaces[i].graph[j]);
        }
        //Now we have the metric space written in. 
        for(int j = 0; j<numInputs[i];j++){
            writer.writeLine("i: ");
            writer.writeLine(inputs[i][j]);
            for(int l = 0;l<NUM_ALGS;l++){
                writer.writeLine("A"+ std::to_string(l) + " c: " + std::to_string(costs[l][counter]));
            }
            counter++;
        }
    }
}

int main(int argc, char ** argv)
{
    auto start = std::chrono::high_resolution_clock::now();
    //FIXME: ok that these are hardcoded?
    char* inputFile = new char[200];
    char* outputFile = new char[200];

    if(parseInput(inputFile, outputFile, argc, argv) == 1)
        return 1;
    //We now know what algorithms to run, have the input file, and
    //output file, and are ready to start getting the data from the
    //input file.
    std::vector<std::vector<std::vector<int> > > inputs;
    std::vector<int> num_inputs;
    std::vector<Mspace> spaces;
    std::vector<int> algsToRun;
    std::vector<int> num_servers;
    std::vector< std::vector <int> > input_lengths;
    int num_spaces = getInput(inputFile, algsToRun, inputs, input_lengths, num_servers, num_inputs,spaces);
    //Now need to run the algs!!
    //First, create a list of alg objects that we will be running.
    int numRunningAlgs = 0;
    for(int i = 0; i<NUM_ALGS; i++)
        numRunningAlgs = numRunningAlgs+algsToRun[i];
    // std::vector <Alg*> runningAlgs;
    //runningAlgs.reserve(numRunningAlgs);

        //TODO: find better way to do this...
    int totalRuns = 0;
    for(int i = 0; i<num_spaces;i++)
        totalRuns = totalRuns+num_inputs[i];
    std::vector <int> a1_costs;
    std::vector <int> a2_costs;
    std::vector <int> a3_costs;
    std::vector <int> a4_costs;
    std::vector <int> a5_costs;
    std::vector <int> a6_costs;

    // if(algsToRun[0] == 1){
    //     runningAlgs.push_back(new RandomAlg);
    //     a1_costs.reserve(totalRuns);
    // }
    // if(algsToRun[1] == 1){
    //     runningAlgs.push_back(new GreedyAlg);
    //     a2_costs.reserve(totalRuns);
    // }
    // if(algsToRun[2] == 1){
    //     runningAlgs.push_back(new OptAlg);
    //     a3_costs.reserve(totalRuns);
    // }
    // if(algsToRun[3] == 1){
    //     runningAlgs.push_back(new WFAlg);
    //     a4_costs.reserve(totalRuns);
    // }
    // if(algsToRun[4] == 1){
    //     runningAlgs.push_back(new DoubleCoverageAlg);
    //     a5_costs.reserve(totalRuns);
    // }

    //Now we know we want to run all of the algorithms in runningAlgs.
    std::vector<std::vector <int> > costs;
    costs.reserve(NUM_ALGS);
    costs.push_back(a1_costs);
    costs.push_back(a2_costs);
    costs.push_back(a3_costs);
    costs.push_back(a4_costs);
    costs.push_back(a5_costs);
    costs.push_back(a6_costs);

    


    for(int i = 0; i< num_spaces; i++){
        std::vector<int> server_locations;
        server_locations.reserve(num_servers[i]);
        for(int j = 0; j<num_servers[i];j++){
            server_locations.push_back(j);
        }
        for(int j = 0; j < num_inputs[i]; j++){
            std::cout << "ran for input "<< j << std::endl;

            RandomAlg ralg;
            GreedyAlg galg;
            OptAlg oalg;
            WFAlg walg;
            DoubleCoverageAlg dalg;
            KCentersAlg kalg;

            ralg.setGraph(spaces[i]);
            ralg.setServers(num_servers[i], server_locations);
            int cost = ralg.runAlg(inputs[i][j], input_lengths[i][j]);
            costs[0].push_back(cost);
            
            galg.setGraph(spaces[i]);
            galg.setServers(num_servers[i], server_locations);
            cost = galg.runAlg(inputs[i][j], input_lengths[i][j]);
            costs[1].push_back(cost);

            oalg.setGraph(spaces[i]);
            oalg.setServers(num_servers[i], server_locations);
            cost = oalg.runAlg(inputs[i][j], input_lengths[i][j]);
            costs[2].push_back(cost);

            walg.setGraph(spaces[i]);
            walg.setServers(num_servers[i], server_locations);
            cost = walg.runAlg(inputs[i][j], input_lengths[i][j]);
            costs[3].push_back(cost);

            dalg.setGraph(spaces[i]);
            dalg.setServers(num_servers[i], server_locations);
            cost = dalg.runAlg(inputs[i][j], input_lengths[i][j]);
            costs[4].push_back(cost);

            kalg.setGraph(spaces[i]);
            kalg.setServers(num_servers[i], server_locations);
            cost = kalg.runAlg(inputs[i][j], input_lengths[i][j]);
            costs[5].push_back(cost);

            // for(int l = 0; l<numRunningAlgs; l++){
            //     //First, set the metric space. 
            //     runningAlgs[l]->setGraph(spaces[i]);
            //     runningAlgs[l]->setServers(num_servers[i],server_locations);
            //     //Now we have the servers set, and the graph set. 
            //     int cost = runningAlgs[l]->runAlg(inputs[i][j], input_lengths[i][j]);
            //     costs[l].push_back(cost);
            // }
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    std::cout << duration.count() <<std::endl;
    //TODO: now need to output a file with all of the data!
    printOutput(outputFile, costs, num_inputs, spaces, inputs, num_servers);
    // for(auto ptr:runningAlgs)
    //     delete ptr;
    delete [] inputFile;
    delete [] outputFile;
    //TODO: need to free mspaces and inputs
    return 0;
}
