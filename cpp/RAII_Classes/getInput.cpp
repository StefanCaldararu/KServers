//author: Stefan Caldararu

#include <fstream>
#include <vector>
#include <string.h>

// class lineParser
// {
//     private:

//     public:
//         std::string line, word;
//         std::vector<std::string> row;
// };

class GetInput
{
    private:
        std::fstream fin;
    public:
        std::string line, word;

        GetInput(char* inputFile){
            openFile(inputFile);
        }

        void openFile(char* inputFile){
            fin.open(inputFile, std::ios::in);
        }
        
        void getLine(){
            getline(fin, line);
        }


        ~GetInput(){
            fin.close();
        }
};
