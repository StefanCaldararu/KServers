//Similar implementation as in "wfa_efficient.cpp" except for all of the algorithms. memoization approach.
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
#include <chrono>

struct cost{
    int WFA;
    int greedy;
    int DC;
    int KC;
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
//Since we have mltiple algorithms, each one needs a different vector of states, to memoize to.
struct all_states{
    std::vector <state> WFA;
    std::vector <state> greedy;
    std::vector <state> DC;
    std::vector <state> KC;
};

bool checkIfCovered(int input, std::vector<int> config){
    for(int i = 0; i<config.size();i++)
        if(config[i] == input)
            return true;
    return false;
}



void runWFA(state &myState)
{
    Mcfp flowNetwork;
    int serverToMove = flowNetwork.setGraph(myState.metricSpace, myState.Sigma, myState.inputLength, myState.k, myState.init_config, myState.config);
    myState.cost += myState.metricSpace.getDistance(myState.config[serverToMove], myState.Sigma[myState.inputLength-1]);
    //moveServer(serverToMove, Sigma[i]);
    myState.config[serverToMove] = myState.Sigma[myState.inputLength-1];
}

void runGreedy(state &myState){
        int input = myState.Sigma[myState.inputLength-1];
        if(!checkIfCovered(input, myState.config)){
            //FIXME: we always move the server w/ smallest ID number to the location in the event of a tie, should we do something else?
            int closest = 0;
            int closest_dist = myState.metricSpace.getDistance(input, myState.config[0]);
            for(int j = 1; j<myState.k; j++){
                int dist = myState.metricSpace.getDistance(input, myState.config[j]);
                if(closest_dist>dist){
                    closest = j;
                    closest_dist = dist;
                }
            }
            myState.cost += closest_dist;
            myState.config[closest] = input;    
        }
    return;
}
void runDC(state &myState){
    return;
}
void runKC(state &myState){
    return;
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
int num_inputs;
std::vector<cost> results;
std::mutex m;
std::condition_variable cv;
int SigmaLength = 8;
std::vector <int> current_input;

void producer_function (int threadID, state theState, Buffer &buffer, int k){
    //first, we need to compute the FIRST state, and make sure we get the correct cost. then output that, and start the while loop.
    // std::vector <state> myStates;
    all_states myStates;
    //for loop, that pushes back each state to myStates increasing the input length each time.
    myStates.DC.push_back(theState);
    myStates.greedy.push_back(theState);
    myStates.KC.push_back(theState);
    myStates.WFA.push_back(theState);
    myStates.DC[0].inputLength = 0;
    myStates.greedy[0].inputLength = 0;
    myStates.KC[0].inputLength = 0;
    myStates.WFA[0].inputLength = 0;
    myStates.DC[0].Sigma.clear();
    myStates.greedy[0].Sigma.clear();
    myStates.KC[0].Sigma.clear();
    myStates.WFA[0].Sigma.clear();

    for(int i = 1;i<SigmaLength+1;i++){
        //WFA
        state newState = myStates.WFA[i-1];
        newState.inputLength = i;
        newState.Sigma.push_back(newState.fullSigma[i-1]);
        //calculate the cost of this state.
        //first, check if the next input is in config. If so, skip running
        bool inConfig = false;
        for(int j = 0;j<newState.config.size();j++)
            if(newState.config[j] == newState.Sigma[i-1])
                inConfig = true;
        if (!inConfig)
            runWFA(newState);
        myStates.WFA.push_back(newState);
        //greedy
        newState = myStates.greedy[i-1];
        newState.inputLength = i;
        newState.Sigma.push_back(newState.fullSigma[i-1]);
        //calculate the cost of this state.
        //first, check if the next input is in config. If so, skip running
        inConfig = false;
        for(int j = 0;j<newState.config.size();j++)
            if(newState.config[j] == newState.Sigma[i-1])
                inConfig = true;
        if (!inConfig)
            runGreedy(newState);
        myStates.greedy.push_back(newState);
        //DC
        newState = myStates.DC[i-1];
        newState.inputLength = i;
        newState.Sigma.push_back(newState.fullSigma[i-1]);
        //calculate the cost of this state.
        //first, check if the next input is in config. If so, skip running
        inConfig = false;
        for(int j = 0;j<newState.config.size();j++)
            if(newState.config[j] == newState.Sigma[i-1])
                inConfig = true;
        if (!inConfig)
            runDC(newState);
        myStates.DC.push_back(newState);
        //KC
        newState = myStates.KC[i-1];
        newState.inputLength = i;
        newState.Sigma.push_back(newState.fullSigma[i-1]);
        //calculate the cost of this state.
        //first, check if the next input is in config. If so, skip running
        inConfig = false;
        for(int j = 0;j<newState.config.size();j++)
            if(newState.config[j] == newState.Sigma[i-1])
                inConfig = true;
        if (!inConfig)
            runKC(newState);
        myStates.KC.push_back(newState);

    }
    //the initial number of replacements we need is 0
    int num_replacements = 0;
    bool run = true;
    //now we have the initial states, and we can start the while loop.
    while(run){
        if(myStates.greedy[SigmaLength].Sigma == theState.endSigma)
            run = false;
        //first, run a for loop to replace the correct number of elements from newStates, given the new input sequence.
        for(int i = 0;i<num_replacements;i++){
            //ci for current index
            int ci = SigmaLength-num_replacements+i;
            //first, update the fullSigma for all algs
            myStates.DC[ci+1].fullSigma = myStates.DC[SigmaLength].fullSigma;
            myStates.greedy[ci+1].fullSigma = myStates.greedy[SigmaLength].fullSigma;
            myStates.KC[ci+1].fullSigma = myStates.KC[SigmaLength].fullSigma;
            myStates.WFA[ci+1].fullSigma = myStates.WFA[SigmaLength].fullSigma;
            //now, copy the Sigma from the previous state
            myStates.DC[ci+1].Sigma = myStates.DC[ci].Sigma;
            myStates.DC[ci+1].Sigma.push_back(myStates.DC[ci+1].fullSigma[ci]);
            myStates.greedy[ci+1].Sigma = myStates.greedy[ci].Sigma;
            myStates.greedy[ci+1].Sigma.push_back(myStates.greedy[ci+1].fullSigma[ci]);
            myStates.KC[ci+1].Sigma = myStates.KC[ci].Sigma;
            myStates.KC[ci+1].Sigma.push_back(myStates.KC[ci+1].fullSigma[ci]);
            myStates.WFA[ci+1].Sigma = myStates.WFA[ci].Sigma;
            myStates.WFA[ci+1].Sigma.push_back(myStates.WFA[ci+1].fullSigma[ci]);
            //copy the cost from the previous state
            myStates.DC[ci+1].cost = myStates.DC[ci].cost;
            myStates.greedy[ci+1].cost = myStates.greedy[ci].cost;
            myStates.KC[ci+1].cost = myStates.KC[ci].cost;
            myStates.WFA[ci+1].cost = myStates.WFA[ci].cost;
            //copy the config from the previous state
            myStates.DC[ci+1].config = myStates.DC[ci].config;
            myStates.greedy[ci+1].config = myStates.greedy[ci].config;
            myStates.KC[ci+1].config = myStates.KC[ci].config;
            myStates.WFA[ci+1].config = myStates.WFA[ci].config;
            //now, run the algorithm on this state
            bool inConfig = false;
            for(int j = 0;j<myStates.DC[ci+1].config.size();j++)
                if(myStates.DC[ci+1].config[j] == myStates.DC[ci+1].Sigma[ci])
                    inConfig = true;
            if (!inConfig)
                runDC(myStates.DC[ci+1]);
            inConfig = false;
            for(int j = 0;j<myStates.greedy[ci+1].config.size();j++)
                if(myStates.greedy[ci+1].config[j] == myStates.greedy[ci+1].Sigma[ci])
                    inConfig = true;
            if (!inConfig)
                runGreedy(myStates.greedy[ci+1]);
            inConfig = false;
            for(int j = 0;j<myStates.KC[ci+1].config.size();j++)
                if(myStates.KC[ci+1].config[j] == myStates.KC[ci+1].Sigma[ci])
                    inConfig = true;
            if (!inConfig)
                runKC(myStates.KC[ci+1]);
            inConfig = false;
            for(int j = 0;j<myStates.WFA[ci+1].config.size();j++)
                if(myStates.WFA[ci+1].config[j] == myStates.WFA[ci+1].Sigma[ci])
                    inConfig = true;    
            if (!inConfig)
                runWFA(myStates.WFA[ci+1]);
        }
        //now, we have the correct cost. add this to the buffer, and increment the sigma, simultanously updating the num_replacements appropriately.
        struct Out c;
        c.input = myStates.WFA[SigmaLength].Sigma;
        c.WFA = myStates.WFA[SigmaLength].cost;
        c.greedy = myStates.greedy[SigmaLength].cost;
        c.DC = myStates.DC[SigmaLength].cost;
        c.KC = myStates.KC[SigmaLength].cost;
        buffer.produce(threadID, c);
        //update the sigma. increment the last element in the vector, modulo the size of the mspace. Then, if it is 0, increment num_replacements and the second to last element in the vector. Repeat until we reach the end of the vector, or we don't need to increment anymore.
        int i = SigmaLength-1;
        num_replacements = 0;
        myStates.DC[SigmaLength].Sigma[i] = (myStates.DC[SigmaLength].Sigma[i]+1)%mspace.getSize();
        num_replacements++;
        while(i>0 && myStates.DC[SigmaLength].Sigma[i] == 0){
            i--;
            myStates.DC[SigmaLength].Sigma[i] = (myStates.DC[SigmaLength].Sigma[i]+1)%mspace.getSize();
            num_replacements++;
        }
        myStates.DC[SigmaLength].fullSigma = myStates.DC[SigmaLength].Sigma;

        i = SigmaLength-1;
        myStates.greedy[SigmaLength].Sigma[i] = (myStates.greedy[SigmaLength].Sigma[i]+1)%mspace.getSize();
        while(i>0 && myStates.greedy[SigmaLength].Sigma[i] == 0){
            i--;
            myStates.greedy[SigmaLength].Sigma[i] = (myStates.greedy[SigmaLength].Sigma[i]+1)%mspace.getSize();
        }
        myStates.greedy[SigmaLength].fullSigma = myStates.greedy[SigmaLength].Sigma;

        i = SigmaLength-1;
        myStates.KC[SigmaLength].Sigma[i] = (myStates.KC[SigmaLength].Sigma[i]+1)%mspace.getSize();
        while(i>0 && myStates.KC[SigmaLength].Sigma[i] == 0){
            i--;
            myStates.KC[SigmaLength].Sigma[i] = (myStates.KC[SigmaLength].Sigma[i]+1)%mspace.getSize();
        }
        myStates.KC[SigmaLength].fullSigma = myStates.KC[SigmaLength].Sigma;

        i = SigmaLength-1;
        myStates.WFA[SigmaLength].Sigma[i] = (myStates.WFA[SigmaLength].Sigma[i]+1)%mspace.getSize();
        while(i>0 && myStates.WFA[SigmaLength].Sigma[i] == 0){
            i--;
            myStates.WFA[SigmaLength].Sigma[i] = (myStates.WFA[SigmaLength].Sigma[i]+1)%mspace.getSize();
        }
        myStates.WFA[SigmaLength].fullSigma = myStates.WFA[SigmaLength].Sigma;


    }
}

void consumer_function(int threadID, WriteOutput& writer, Buffer &buffer){
    for(int i = 0;i<num_inputs;i++){
        // std::cout << "write1: " << i << std::endl;
        Out results = buffer.consume(i);
        writer.writeLine("inp:");
        writer.writeLine(results.input);
        writer.writeLine("WFA: "+std::to_string(results.WFA));
        writer.writeLine("greedy: "+std::to_string(results.greedy));
        writer.writeLine("DC: "+std::to_string(results.DC));
        writer.writeLine("KC: "+std::to_string(results.KC));
    }

}

int main(int argc, char ** argv)
{
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
    std::vector <int> input;
    std::vector <int> end;
    input.push_back(0);
    end.push_back(size-1);

    for (int i = 0; i < SigmaLength-1; i++){
        input.push_back(0);
        end.push_back(size-1);
    }

    //and number of servers...
    reader.getLine();
    int num_servers = std::stoi(reader.line);
    mspace = space;

    std::vector <int> config;
    for (int i = 0; i < num_servers; i++){
        config.push_back(i);
    }
    std::vector <int> init_config = config;

    //construct the initial state
    state theState;
    theState.metricSpace = space;
    //instantiate sigma to an empty vector
    theState.Sigma = std::vector<int>();
    theState.fullSigma = input;
    theState.endSigma = end;
    theState.inputLength = 0;
    theState.k = num_servers;
    theState.cost = 0;
    theState.init_config = init_config;
    theState.config = config;
    //now, create a vector of states, one for each thread.
    std::vector <state> myStates;
    for(int i = 0;i<size;i++){
        state newState = theState;
        newState.fullSigma[0] = i;
        newState.endSigma[0] = i;
        myStates.push_back(newState);
    }




    Buffer buffer;

    //start a timer... with chrono
    auto start = std::chrono::high_resolution_clock::now();

    //create the producer threads....
    std::vector<std::thread> producerThreads;
    for(int i = 0;i<size;i++)
        producerThreads.emplace_back(producer_function, i, myStates[i], std::ref(buffer), num_servers);
    
    //
    WriteOutput writer(outputFile);
    std::thread consumerThread(consumer_function, size, std::ref(writer), std::ref(buffer));

    //join the threads...
    for(auto& thread : producerThreads)
        thread.join();
    consumerThread.join();
    //Now we have the number of inputs. want to create a child to output stuff, and child threads to process inputs...


    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
    std::cout << duration.count() <<std::endl;
    //TODO: now need to output a file with all of the data!
    
    delete [] inputFile;
    delete [] outputFile;
    //TODO: need to free mspaces and inputs
    return 0;
}
