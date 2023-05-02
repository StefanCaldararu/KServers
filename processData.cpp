//autor: Stefan Caldararu
#include <iostream>
#include <string.h> 
#include <fstream>
#include <vector>
#include <sstream>
#include <omp.h>
#include <algorithm>
#include "RAII_Classes/writeOutput.cpp"

int NUM_ALGS = 6;

int parseInput(char* inputFile, int argc, char** argv)
{
    for(int i = 0; i<argc; i++){
        if(strcmp(argv[i], "-input") == 0 || strcmp(argv[i], "-i") == 0){
            if(i+1<=argc)
                strcpy(inputFile, argv[i+1]);
            else
                return 1;
        }
    }
    return 0;
}
//Returns the number of mspaces, or if doesn't work returns -1.
//TODO: create object for this whole function?
int getInput(char* inputFile, std::vector<std::vector<int> >& results)

//std::vector<int>& algsToRun, std::vector <std::vector<std::vector<int> > >& inputs,std::vector<std::vector<int> >& input_lengths, std::vector<int >& num_servers, std::vector<int>& num_inputs, std::vector <Mspace>& spaces)
{
    int size = 0;
    std::fstream file(inputFile);
    if(file.is_open()){
        std::string line;
        while(std::getline(file, line)){
            std::getline(file, line);
            for(int i = 0;i<NUM_ALGS;i++){
                std::getline(file, line);
                std::size_t pos = line.find(':');
                std::string word = line.substr(pos+1);
                int value = std::stoi(word);
                //if(size > 2999999 && size<4000000)
                results[i].push_back(value);
            }
            size++;

        }
    }
    else{
        std::cerr << "UNABLE TO OPEN FILE!" << std::endl;
        return 1;
    }
    file.close();
    return size;

}
int main(int argc, char ** argv)
{
    //auto start = std::chrono::high_resolution_clock::now();
    //FIXME: ok that these are hardcoded?
    char* inputFile = new char[200];

    if(parseInput(inputFile, argc, argv) == 1)
        return 1;
    std::vector<std::vector<int> > results;
    for(int i = 0;i<NUM_ALGS;i++)
        results.push_back(std::vector<int>()); 
    int size = getInput(inputFile, results);

    double GO_MM = 0;
    double WO_MM = 0;
    double WG_MM = 0;
    int length = results[0].size();
    for(int i = 0;i<results[0].size();i++){
        if((double)results[1][i]/(double)results[2][i]>GO_MM)
            GO_MM = (double)results[1][i]/(double)results[2][i];
        if((double)results[3][i]/(double)results[2][i]>WO_MM)
            WO_MM = (double)results[3][i]/(double)results[2][i];
        if((double)results[3][i]/(double)results[1][i]>WG_MM)
            WG_MM = (double)results[3][i]/(double)results[1][i];
    }

    //#pragma omp parallel for
    for (size_t i = 0;i<NUM_ALGS;i++){
        std::sort(results[i].begin(), results[i].end());
    }
    //Alg 0 is rand
    //Alg 1 is greedy
    //Alg 2 is opt
    //Alg 3 is WFA
    //Alg 4 is double coverage
    //Alg 5 is kcenters
    double greedy_BA = 0;
    double WFA_BA = 0;
    double DC_BA = 0;
    double KC_BA = 0;
    double GW_BA = 0;
    double WG_BA = 0;
    for(int i = 0;i<results[0].size();i++){
        if((double)results[1][i]/(double)results[2][i] > greedy_BA){
            greedy_BA = (double)results[1][i]/(double)results[2][i];
        }
        if((double)results[3][i]/(double)results[2][i] > WFA_BA){
            WFA_BA = (double)results[3][i]/(double)results[2][i];
        }
        if((double)results[4][i]/(double)results[2][i] > DC_BA){
            DC_BA = (double)results[4][i]/(double)results[2][i];
        }
        if((double)results[5][i]/(double)results[2][i] > KC_BA){
            KC_BA = (double)results[5][i]/(double)results[2][i];
        }
        if((double)results[1][i]/(double)results[3][i] > GW_BA)
            GW_BA = (double)results[1][i]/(double)results[3][i];
        if((double)results[3][i]/(double)results[1][i]> WG_BA)
            WG_BA = (double)results[3][i]/(double)results[1][i];
    }


    std::cout <<"GREEDY BA: " << greedy_BA << std::endl;
    std::cout <<"WFA BA: " << WFA_BA << std::endl;
    std::cout << "WFA/GREEDY BA: " << WG_BA << std::endl;
    std::cout << "MM GO: " << (double)results[1][length-1]/(double)results[2][length-1] << std::endl;
    std::cout << "MM WO: " << (double)results[3][length-1]/(double)results[2][length-1] << std::endl;
    std::cout << "MM WG: " << (double)results[3][length-1]/(double)results[1][length-1] << std::endl;
    std::cout << "DA GO: " << GO_MM << std::endl;
    std::cout << "DA WO: " << WO_MM << std::endl;
    std::cout << "DA WG: " << WG_MM << std::endl;
    
    // std::cout <<"DC BA: " << DC_BA << std::endl;
    // std::cout <<"KC BA: " << KC_BA << std::endl;
    // std::cout << "GREEDY/WFA BA: " << GW_BA << std::endl;

    return 0;
}