//autor: Stefan Caldararu

//In this version, we assume there is only ONE MSPACE being input. 
//This will 
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

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
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

void printOutput(char* outputFile, std::vector<std::vector<int> >& costs, std::vector<int>& numInputs, std::vector<Mspace>& metricSpaces, std::vector<std::vector<std::vector<int> > >& inputs, std::vector<int> num_servers){
    WriteOutput writer(outputFile);
    int counter = 0;
    for(int i = 0;i<metricSpaces.size();i++){

        writer.writeLine("mspace:");
        writer.writeLine("k: "+ std::to_string(num_servers[i]));
        for(int j = 0;j<metricSpaces[i].getSize();j++){
            writer.writeLine(metricSpaces[i].graph[j]);
        }
        //Now we have the metric space written in. 
        for(int j = 0; j<numInputs[i];j++){
            writer.writeLine("inp:");
            writer.writeLine(inputs[i][j]);
            for(int l = 0;l<NUM_ALGS;l++){
                writer.writeLine("A "+ std::to_string(l) + " c " + std::to_string(costs[l][counter]));
            }
            counter++;
        }
    }
}

int location = 0;
int writerLocation = 0;
std::queue<std::vector<int> > queue;
std::mutex m;
std::condition_variable cv;
//Default holds -1 in the 0th position, until all of the others are filled in. Each alg has a position to put it's cost, from 1 to 6
std::vector<int> DEFAULT;
DEFAULT.push_back(-1);
for(int i = 0;i<6;i++)
    DEFAULT.push_back(0);

void producer_function (Mspace mspace, int threadID, GetInput& reader, int num_inputs){
    std::unique_lock<std::mutex> queueLock(m);
    int myloc = location;
    while(location<num_inputs-1){
        reader.getLine();
        std::vector<int> input;
        reader.getLine();
        std::string word;
        std::stringstream str(reader.line);
        while(getline(str, word, ','))
            input.push_back(stoi(word));
        //Now we have the input...
        location++;
        queue.push(DEFAULT);
        queueLock.unlock();
        std::vector<int> out;
        out.push_back(myloc);
        
        //TODO: Get the costs of the algs, and push_back to out...

        queueLock(m);

        
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



    //Here we are assuming that ther eis only one mspace, and each line has a new input. We will be processing the inputs one at a time, and having one consumer thread, multiple producer threads.


    GetInput reader(inputFile);
    //The algs we want to run
    reader.getLine();
    //The number of mspaces(1)
    reader.getLine();
    Mspace space;
    //Number of nodes for this input, size of the mspace
    reader.getLine();
    int size = std::stoi(reader.line);
    space.setSize(size);
    std::string word;
    //populate the mspace!
    for(int j = 0; j<size; j++){
        reader.getLine();
        std::stringstream str(reader.line);
        int k = 0;
        while(getline(str, word, ',')){
            space.setDistance(j, k, stoi(word));
            k++;
        }
    }
    //Now we have the mspace loaded in... get the number of inputs..
    reader.getLine();
    int NI = std::stoi(reader.line);
    //Now we have the number of inputs. want to create a child to output stuff, and child threads to process inputs...
    //global variables:
    std::queue<std::vector<int> > queue;
    std::mutex m;
    std::condition_variable cv;



    

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

    std::vector <std::vector<int> > a1_costs;
    std::vector <std::vector<int> > a2_costs;
    std::vector <std::vector<int> > a3_costs;
    std::vector <std::vector<int> > a4_costs;
    std::vector <std::vector<int> > a5_costs;
    std::vector <std::vector<int> > a6_costs;


    std::string folder_name(outputFile);
    //std::filesystem::create_directory(folder_name);
    std::ofstream randFile("rand_output.csv");
    std::ofstream greedyFile("greedy_output.csv");
    std::ofstream optFile1("opt1_output.csv");
    std::ofstream optFile2("opt2_output.csv");
    std::ofstream wfaFile1("wfa1_output.csv");
    std::ofstream wfaFile2("wfa2_output.csv");
    std::ofstream doubleCoverageFile("doubleCoverage_output.csv");
    std::ofstream kcentersFile("kcenters_output.csv");
    #pragma omp parallel for num_threads(8)
    for(int l = 0;l<8;l++){
        if(l == 0){
            for(int i = 0;i<num_spaces;i++){
                std::vector<int> server_locations;
                server_locations.reserve(num_servers[i]);
                for(int j = 0; j<num_servers[i];j++){
                    server_locations.push_back(j);
                }
                for(int j = 0;j<num_inputs[i];j++){
                    std::cout << "ran for input "<< j << std::endl;
                    RandomAlg alg;
                    alg.setGraph(spaces[i]);
                    alg.setServers(num_servers[i], server_locations);
                    int cost = alg.runAlg(inputs[i][j], input_lengths[i][j]);
                    randFile << cost << "\n";

                }

            }

        }
        if(l == 1){
            for(int i = 0;i<num_spaces;i++){
                std::vector<int> server_locations;
                server_locations.reserve(num_servers[i]);
                for(int j = 0; j<num_servers[i];j++){
                    server_locations.push_back(j);
                }
                for(int j = 0;j<num_inputs[i];j++){
                    std::cout << "ran for input "<< j << std::endl;
                    GreedyAlg alg;
                    alg.setGraph(spaces[i]);
                    alg.setServers(num_servers[i], server_locations);
                    int cost = alg.runAlg(inputs[i][j], input_lengths[i][j]);
                    greedyFile << cost << "\n";

                }

            }
        }
        if(l == 2){
            for(int i = 0;i<(num_spaces/2)+1;i++){
                std::vector<int> server_locations;
                server_locations.reserve(num_servers[i]);
                for(int j = 0; j<num_servers[i];j++){
                    server_locations.push_back(j);
                }
                for(int j = 0;j<num_inputs[i];j++){
                    std::cout << "ran for input "<< j << std::endl;
                    OptAlg alg;
                    alg.setGraph(spaces[i]);
                    alg.setServers(num_servers[i], server_locations);
                    int cost = alg.runAlg(inputs[i][j], input_lengths[i][j]);
                    optFile1 << cost << "\n";
                }

            }
        }
        if( l == 3){
            for(int i = (num_spaces/2)+1;i<num_spaces;i++){
                std::vector<int> server_locations;
                server_locations.reserve(num_servers[i]);
                for(int j = 0; j<num_servers[i];j++){
                    server_locations.push_back(j);
                }
                for(int j = 0;j<num_inputs[i];j++){
                    std::cout << "ran for input "<< j << std::endl;
                    OptAlg alg;
                    alg.setGraph(spaces[i]);
                    alg.setServers(num_servers[i], server_locations);
                    int cost = alg.runAlg(inputs[i][j], input_lengths[i][j]);
                    optFile2 << cost << "\n";
                }

            }
        }
        if(l == 4){
            for(int i = 0;i<(num_spaces/2)+1;i++){
                std::vector<int> server_locations;
                server_locations.reserve(num_servers[i]);
                for(int j = 0; j<num_servers[i];j++){
                    server_locations.push_back(j);
                }
                for(int j = 0;j<num_inputs[i];j++){
                    std::cout << "ran for input "<< j << std::endl;
                    WFAlg alg;
                    alg.setGraph(spaces[i]);
                    alg.setServers(num_servers[i], server_locations);
                    int cost = alg.runAlg(inputs[i][j], input_lengths[i][j]);
                    wfaFile1 << cost << "\n";

                }

            }
        }
        if(l == 5){
            for(int i = (num_spaces/2)+1;i<num_spaces;i++){
                std::vector<int> server_locations;
                server_locations.reserve(num_servers[i]);
                for(int j = 0; j<num_servers[i];j++){
                    server_locations.push_back(j);
                }
                for(int j = 0;j<num_inputs[i];j++){
                    std::cout << "ran for input "<< j << std::endl;
                    WFAlg alg;
                    alg.setGraph(spaces[i]);
                    alg.setServers(num_servers[i], server_locations);
                    int cost = alg.runAlg(inputs[i][j], input_lengths[i][j]);
                    wfaFile2 << cost << "\n";
                }
            }
        }
        if(l == 6){
            for(int i = 0;i<num_spaces;i++){
                std::vector<int> server_locations;
                server_locations.reserve(num_servers[i]);
                for(int j = 0; j<num_servers[i];j++){
                    server_locations.push_back(j);
                }
                for(int j = 0;j<num_inputs[i];j++){
                    std::cout << "ran for input "<< j << std::endl;
                    DoubleCoverageAlg alg;
                    alg.setGraph(spaces[i]);
                    alg.setServers(num_servers[i], server_locations);
                    int cost = alg.runAlg(inputs[i][j], input_lengths[i][j]);
                    doubleCoverageFile << cost << "\n";

                }

            }
        }
        if(l == 7){
            for(int i = 0;i<num_spaces;i++){
                std::vector<int> server_locations;
                server_locations.reserve(num_servers[i]);
                for(int j = 0; j<num_servers[i];j++){
                    server_locations.push_back(j);
                }
                for(int j = 0;j<num_inputs[i];j++){
                    std::cout << "ran for input "<< j << std::endl;
                    KCentersAlg alg;
                    alg.setGraph(spaces[i]);
                    alg.setServers(num_servers[i], server_locations);
                    int cost = alg.runAlg(inputs[i][j], input_lengths[i][j]);
                    kcentersFile << cost << "\n";
                }
            }
        }
    }


    randFile.close();
    greedyFile.close();
    optFile1.close();
    optFile2.close();
    wfaFile1.close();
    wfaFile2.close();
    doubleCoverageFile.close();
    kcentersFile.close();


    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    std::cout << duration.count() <<std::endl;
    //TODO: now need to output a file with all of the data!
    
    delete [] inputFile;
    delete [] outputFile;
    //TODO: need to free mspaces and inputs
    return 0;
}
