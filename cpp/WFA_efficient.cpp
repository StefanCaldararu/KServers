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

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <iostream>
#include <string.h> 
#include <fstream>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <cmath>
#include "RAII_Classes/getInput.cpp"
#include "RAII_Classes/writeOutput.cpp"
#include "RAII_Classes/out.h"
#include "RAII_Classes/buffer.hpp"
#include "RAII_Classes/mcfp.cpp"

const int NUM_ALGS = 6;
struct cost{
    int location;
    int WFA;
    std::vector<int> input;
};
//At each stage of the algorithm, we have a certain "state" we want to keep track of.
struct state{
    int cost;
    std::vector <int> init_config;
    std::vector <int> config;
    Mspace metricSpace;
    std::vector <int> Sigma;
    std::vector <int> fullSigma;
    std::vector <int> endSigma;
    int inputLength;
    int k;
};


void runAlg(state &myState)
{
    Mcfp flowNetwork;
    int serverToMove = flowNetwork.setGraph(myState.metricSpace, myState.Sigma, myState.inputLength, myState.k, myState.init_config, myState.config);
    myState.cost += myState.metricSpace.getDistance(myState.config[serverToMove], myState.Sigma[myState.inputLength]);
    //moveServer(serverToMove, Sigma[i]);
    myState.config[serverToMove] = myState.Sigma[myState.inputLength];
}

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

Mspace mspace;
int location = 0;
int num_inputs;
std::vector<cost> results;
std::mutex m;
std::condition_variable cv;
int SigmaLength = 20;
std::vector <int> current_input;

void producer_function (int threadID, state theState, Buffer &buffer, int k){
    //first, we need to compute the FIRST state, and make sure we get the correct cost. then output that, and start the while loop.
    std::vector <state> myStates;
    //for loop, that pushes back each state to myStates increasing the input length each time.
    myStates.push_back(theState);
    myStates[0].inputLength = 1;
    myStates[0].Sigma.clear();
    myStates[0].Sigma.push_back(myStates[0].fullSigma[0]);
    for(int i = 1;i<SigmaLength;i++){
        state newState = myStates[i-1];
        newState.inputLength = i+1;
        newState.Sigma.push_back(newState.fullSigma[i]);
        //calculate the cost of this state.
        runAlg(newState);
        myStates.push_back(newState);
    }
    //the initial number of replacements we need is 0
    int num_replacements = 0;
    //now we have the initial states, and we can start the while loop.
    while(myStates[SigmaLength].Sigma != theState.endSigma){
        //first, run a for loop to replace the correct number of elements from newStates, given the new input sequence.
        for(int i = 0;i<num_replacements;i++){
            //first, update the fullSigma
            myStates[SigmaLength-num_replacements+i].fullSigma = myStates[SigmaLength].fullSigma;
            //replace the length of the input -num_replacements +i
            //TODO: TODO: TODO: we are here
        }
        //myStates[sigmaLength].sigma gets updated at the end
    }

    while(startState.Sigma != startState.endSigma){
        reader.getLine();
        std::vector<int> input;
        std::string word;
        std::stringstream str(reader.line);
        while(getline(str, word, ','))
            input.push_back(stoi(word));
        //Now we have the input...
        location++;
        queueLock.unlock();



        struct Out c;
        c.input = input;

        std::vector<int> server_locations;
        server_locations.reserve(k);
        for(int j = 0;j<k;j++)
            server_locations.push_back(j);
        
        WFAlg walg;

        int size = input.size();

        walg.setGraph(mspace);
        walg.setServers(k, server_locations);
        c.WFA = walg.runAlg(input, size);


        buffer.produce(threadID, c);
        queueLock.lock();

        
        
    }
    queueLock.unlock();
}

void consumer_function(int threadID, WriteOutput& writer, Buffer &buffer){
    for(int i = 0;i<num_inputs;i++){
        // std::cout << "write1: " << i << std::endl;
        Out results = buffer.consume(i);
        writer.writeLine("inp:");
        writer.writeLine(results.input);
        writer.writeLine("cost: "+std::to_string(results.WFA));
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

    //populate the mspace!
    for(int j = 0; j<size; j++){
        reader.getLine();
        std::stringstream str(reader.line);
        std::string word;

        int k = 0;
        while(getline(str, word, ',')){
            space.setDistance(j, k, std::stoi(word));
            k++;
        }
    }
    //Now we have the mspace loaded in... get the number of inputs..
    reader.getLine();
    //the number of inputs is size to the power SigmaLength
    num_inputs = std::pow(size, SigmaLength);
    for (int i = 0; i < SigmaLength; i++)
        current_input.push_back(0);


    //and number of servers...
    reader.getLine();
    int num_servers = std::stoi(reader.line);
    mspace = space;


    Buffer buffer;

    //create the producer threads....
    std::vector<std::thread> producerThreads;
    for(int i = 0;i<20;i++)
        producerThreads.emplace_back(producer_function, i, std::ref(reader), std::ref(buffer), num_servers);
    
    //
    WriteOutput writer(outputFile);
    std::thread consumerThread(consumer_function, 20, std::ref(writer), std::ref(buffer));

    //join the threads...
    for(auto& thread : producerThreads)
        thread.join();
    consumerThread.join();
    //Now we have the number of inputs. want to create a child to output stuff, and child threads to process inputs...


    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    //std::cout << duration.count() <<std::endl;
    //TODO: now need to output a file with all of the data!
    
    delete [] inputFile;
    delete [] outputFile;
    //TODO: need to free mspaces and inputs
    return 0;
}
