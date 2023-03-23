//author: Stefan Caldararu

#include <fstream>
#include <vector>
#include <string.h>

class lineParser
{
    private:

    public:
        std::string line, word;
        std::vector<std::string> row;
};

class getInput
{
    private:

    public:
        std::fstream fin;
        getInput(char* inputFile){
            fin.open(inputFile, std::ios::in);

        }

        ~getInput(){
            fin.close();
        }
};
