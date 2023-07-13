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

class WriteOutput
{
    private:
        std::ofstream fout;
    public:
        WriteOutput(char* outputFile){
            openFile(outputFile);
        }
        void openFile(char* outputFile){
            fout.open(outputFile);
        }
        void writeLine(std::vector<int>line){
            for(int i = 0;i<line.size();i++)
                fout << line[i] << ",";
            fout<< "\n";
        }
        void writeLine(std::string line){
            fout << line << "\n";
        }


        ~WriteOutput(){
            fout.close();
        }
};
