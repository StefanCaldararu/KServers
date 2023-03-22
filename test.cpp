//author: Stefan Caldararu

#include "randomAlg.h"
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>

int main(int argc, char ** argv)
{
    char* inputFile = new char[100];
    strcpy(inputFile, argv[1]);

    
    std::fstream fin;
    fin.open(inputFile, std::ios::in);
    std::vector<std::string> row;
    std::string line, word;
    //The first line is the number of mspaces
    getline(fin, line);
    std::cout << line << "\n";

    delete(inputFile);
    // Mspace mSpace(2);
    // mSpace.setDistance(0,0,1);
    // mSpace.setDistance(0,1,1);
    // mSpace.setDistance(1,0,1);
    // mSpace.setDistance(1,1,1);
    // RandomAlg algorithm(mSpace, 2);
    // int Sigma [] = {0,1,1,0};
    // int config[] = {0,0};
    // std::cout << algorithm.runAlg(Sigma, 4, config);
    // // std::cout << mSpace.getDistance(1,1);
    return 0;
}