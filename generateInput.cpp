#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include "RAII_Classes/writeOutput.cpp"
#include <cmath>
using namespace std;

void generate_combinations(vector<int>& current, int length, ofstream& file) {
    if (current.size() == length) {
        for (int i = 0; i < length - 1; i++) {
            file << current[i] << ",";
        }
        file << current[length - 1] << endl;
        return;
    }
    for (int i = 0; i < 10; i++) {
        current.push_back(i);
        generate_combinations(current, length, file);
        current.pop_back();
    }
}

int main() {
    ofstream file;
    file.open("file.csv");
    int line_length = 6;
    int num_lines = 1000000;
    file << "1,1,1,1,1,1" <<endl;
    file << 4 <<endl;
    int graph_size = 10;
    for(int k = 0;k<4;k++){
        file << graph_size << endl;
        for(int i = 0;i<graph_size;i++){
            for(int j = 0;j<graph_size;j++){
                int value = abs(j-i);
                file << value;
                if(j<graph_size-1)
                    file << ",";
            }
            file << endl;
        }
        file << num_lines <<endl;
        file << k+2<<endl;
        vector<int> current;
        generate_combinations(current, line_length, file);
    }
    // srand(time(NULL));
    // for (int i = 0; i < num_lines; i++) {
    //     for (int j = 0; j < line_length; j++) {
    //         int digit = rand() % graph_size;
    //         file << digit;
    //         if (j < line_length - 1) {
    //             file << ",";
    //         }
    //     }
    //     file << endl;
    // }
    
    file.close();
    return 0;
}
