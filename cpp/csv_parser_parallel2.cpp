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
#include <unistd.h>
#include "RAII_Classes/getInput.cpp"
#include "RAII_Classes/writeOutput.cpp"
#include "RAII_Classes/out.h"
#include "RAII_Classes/buffer.hpp"

const int NUM_ALGS = 6;
struct cost{
    int location;
    int rand;
    int greedy;
    int OPT;
    int WFA;
    int DC;
    int KC;
    std::vector<int> input;
};
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
int asdf = 0;

void producer_function (int threadID, GetInput& reader, Buffer &buffer, int k){
    //int myloc = location;
    std::unique_lock<std::mutex> queueLock(m);
    while(location<num_inputs){
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
        
        RandomAlg ralg;
        GreedyAlg galg;
        OptAlg oalg;
        WFAlg walg;
        DoubleCoverageAlg dalg;
        KCentersAlg kalg;

        int size = input.size();
        ralg.setGraph(mspace);
        ralg.setServers(k, server_locations);
        c.rand = ralg.runAlg(input, size);
        
        galg.setGraph(mspace);
        galg.setServers(k, server_locations);
        c.greedy = galg.runAlg(input, size);
        
        oalg.setGraph(mspace);
        oalg.setServers(k, server_locations);
        c.OPT = oalg.runAlg(input, size);

        walg.setGraph(mspace);
        walg.setServers(k, server_locations);
        c.WFA = walg.runAlg(input, size);

        dalg.setGraph(mspace);
        dalg.setServers(k, server_locations);
        c.DC = dalg.runAlg(input, size);

        kalg.setGraph(mspace);
        kalg.setServers(k, server_locations);
        c.KC = kalg.runAlg(input, size);

        buffer.produce(threadID, c);
        queueLock.lock();

        std::cout << "produced: " << location << std::endl;
        //now want to put cost in the right spot of the list...
        //FIXME: false sharing, maybe use multiple buckets so only consumer has this prblem, which runs fast anyways?
        
    }
    queueLock.unlock();
}

void consumer_function(int threadID, WriteOutput& writer, Buffer &buffer){
    for(int i = 0;i<num_inputs;i++){
        // std::cout << "write1: " << i << std::endl;
        Out results = buffer.consume(i);
        writer.writeLine("inp:");
        writer.writeLine(results.input);
        writer.writeLine("A 0 c: "+std::to_string(results.rand));
        writer.writeLine("A 1 c: "+std::to_string(results.greedy));
        writer.writeLine("A 2 c: "+std::to_string(results.OPT));
        writer.writeLine("A 3 c: "+std::to_string(results.WFA));
        writer.writeLine("A 4 c: "+std::to_string(results.DC));
        writer.writeLine("A 5 c: "+std::to_string(results.KC));
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
    num_inputs = std::stoi(reader.line);

    //and number of servers...
    reader.getLine();
    int num_servers = std::stoi(reader.line);
    mspace = space;


    Buffer buffer;

    //create the producer threads....
    std::vector<std::thread> producerThreads;
    for(int i = 0;i<16;i++)
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
    std::cout << duration.count() <<std::endl;
    //TODO: now need to output a file with all of the data!
    
    delete [] inputFile;
    delete [] outputFile;
    //TODO: need to free mspaces and inputs
    return 0;
}
