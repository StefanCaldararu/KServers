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
    int line_length = 100;
    int num_lines = 1000;
    file << "1,1,1,1,1" <<endl;
    file << "1"<<endl;
    file << "10"<<endl;
    file << "0,1,2,3,4,5,6,7,8,9"<<endl;
    file << "1,0,1,2,3,4,5,6,7,8"<<endl;
    file << "2,1,0,1,2,3,4,5,6,7"<<endl;
    file << "3,2,1,0,1,2,3,4,5,6"<<endl;
    file << "4,3,2,1,0,1,2,3,4,5"<<endl;
    file << "5,4,3,2,1,0,1,2,3,4"<<endl;
    file << "6,5,4,3,2,1,0,1,2,3"<<endl;
    file << "7,6,5,4,3,2,1,0,1,2"<<endl;
    file << "8,7,6,5,4,3,2,1,0,1"<<endl;
    file << "9,8,7,6,5,4,3,2,1,0"<<endl;
    file << num_lines <<endl;
    file << "3 "<<endl;
    srand(time(NULL));
    for (int i = 0; i < num_lines; i++) {
        for (int j = 0; j < line_length; j++) {
            int digit = rand() % 10;
            file << digit;
            if (j < line_length - 1) {
                file << ",";
            }
        }
        file << endl;
    }
    
    file.close();
    return 0;
}
