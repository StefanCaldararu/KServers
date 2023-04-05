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

#include "randomAlg.h"
#include "greedyAlg.h"
#include <iostream>
#include <string.h> 
#include <fstream>
#include <vector>
#include <sstream>
#include "RAII_Classes/getInput.cpp"


const int NUM_ALGS = 2;
//This takes in the argv and parses it for the main function. 
//gives pointer to input file, output file, and there is an array of which algorithms to run.
//TODO: probably want to create object for this.
//TODO: move the -algorithms option into the file.
int parseInput(char* inputFile, char*outputFile, int * runAlgs, int argc, char** argv)
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
        if(strcmp(argv[i], "-algorithms") == 0 || strcmp(argv[i], "-a") == 0){
            if(i+1<=argc)
                for(int j = 0;j<NUM_ALGS; j++)
                    runAlgs[j] = std::stoi(std::string(1,argv[i+1][j]));
            else
                return 1;
        }
    }
    return 0;
}
//Returns the number of mspaces, or if doesn't work returns -1.
//TODO: create object for this whole function?
int getInput(char* inputFile,std::vector<int>& algsToRun, std::vector <std::vector<int> >& inputs, std::vector<int>& num_inputs, std::vector <Mspace>& spaces)
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


    reader.getLine();
    int num_mspaces = std::stoi(reader.line);
    spaces.reserve(num_mspaces);
    num_inputs.reserve(num_mspaces);
    inputs.reserve(num_mspaces);
    for(int i = 0; i<num_mspaces; i++){
        reader.getLine();
        int size = std::stoi(reader.line);
        spaces.push_back(Mspace());
        spaces[i].setSize(size);
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
        reader.getLine();
        int NI = std::stoi(reader.line);
        num_inputs.push_back(NI);
        for(int j = 0; j<NI; j++){
            std::vector<int> input;
            input.reserve(NI);
            reader.getLine();
            std::string word;
            std::stringstream str(reader.line);
            while(getline(str, word, ','))
                input.push_back(stoi(word));
            inputs.push_back(input);
        }
    }
    return num_mspaces;

}

int main(int argc, char ** argv)
{
    //FIXME: ok that these are hardcoded?
    char* inputFile = new char[200];
    char* outputFile = new char[200];
    int runAlgs[NUM_ALGS];

    if(parseInput(inputFile, outputFile, runAlgs, argc, argv) == 1)
        return 1;
    //We now know what algorithms to run, have the input file, and
    //output file, and are ready to start getting the data from the
    //input file.
    std::vector<std::vector<int> > inputs;
    std::vector<int> num_inputs;
    std::vector<Mspace> spaces;
    std::vector<int> algsToRun;
    int num_spaces = getInput(inputFile, algsToRun, inputs, num_inputs,spaces);
    if(num_spaces!=-1){
        std::cout << "the distance of first metric space, 0,2 should be 2: " << spaces[0].getDistance(0,2) << "\n";
    
    }

    std::cout << "the input file is: " << inputFile << "\n";
    delete [] inputFile;
    delete [] outputFile;
    //Now need to run the algs!!
    //First, create a list of alg objects that we will be running.
    int numRunningAlgs = 0;
    for(int i = 0; i<NUM_ALGS; i++)
        numRunningAlgs = numRunningAlgs+algsToRun[i];
    std::vector <Alg*> runningAlgs;
    runningAlgs.reserve(numRunningAlgs);

        //TODO: find better way to do this...
    int totalRuns = 0;
    for(int i = 0; i<num_spaces;i++)
        totalRuns = totalRuns+num_inputs[i];
    std::vector <int> a1_costs;
    std::vector <int> a2_costs;

    if(algsToRun[0] == 1)
        runningAlgs.push_back(new RandomAlg());
        a1_costs.reserve(totalRuns);

    if(algsToRun[1] == 1)
        runningAlgs.push_back(new GreedyAlg());
        a2_costs.reserve(totalRuns);
    //Now we know we want to run all of the algorithms in runningAlgs.
    std::vector<std::vector <int> > costs;
    costs.reserve(2);
    costs.push_back(a1_costs);
    costs.push_back(a2_costs);

    for(int i = 0; i< num_spaces; i++){
        for(int j = 0; j < num_inputs[i]; j++){
            for(int l = 0; l<numRunningAlgs; l++){
                //First, set the metric space. 
                runningAlgs[l]->setGraph(spaces[i]);
                //TODO: Fix this pronto! actually set the servers and number of servers correctly.
                std::vector<int> temp;
                temp.reserve(1);
                temp.push_back(1);
                runningAlgs[l]->setServers(1,temp);
                //Now we have the servers set, and the graph set. 
                int cost = runningAlgs[l]->runAlg(inputs[i], num_inputs[i]);
                costs[l].push_back(cost);
            }
        }
    }
    //TODO: now need to output a file with all of the data!

    //TODO: need to free mspaces and inputs
    return 0;
}
