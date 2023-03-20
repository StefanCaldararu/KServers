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
#include <iostream>
#include <string.h> 

const int NUM_ALGS = 1;
//This takes in the argv and parses it for the main function. 
//gives pointer to input file, output file, and there is an array of which algorithms to run.
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

int main(int argc, char ** argv)
{
    char* inputFile;
    char* outputFile;
    int runAlgs[NUM_ALGS];

    if(parseInput(inputFile, outputFile, runAlgs, argc, argv) == 1)
        return 1;
    
    std::cout << "the input file is: " << outputFile << "\n";
    std::cout << "runAlgs[0] is: " << runAlgs[0] << "\n";

    return 0;
}
