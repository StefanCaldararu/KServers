#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include "../cpp/RAII_Classes/writeOutput.cpp"
#include <cmath>
using namespace std;

int GRAPH_SIZE = 3;
int LINE_LENGTH = 6;
int NUM_LINES = 1000000;
int k = 3;
int NUM_THREADS = 1;
int INP_PER_THREAD = NUM_LINES/NUM_THREADS;
int threads_used = 0;
bool printed = false;

void printMspace(ofstream& file){
    file << GRAPH_SIZE << endl;
    //The ceter node is node 0, all other nodes are points.
    for(int i = 0;i<GRAPH_SIZE;i++){
        for(int j = 0;j<GRAPH_SIZE;j++){
            if(i == j)
                file << 0;
            else if(j/3 == i/3)
            //we are on the same line
                file << std::abs(i-j);
            else{
            //we are not on the same line...
                int count = 0;
                if(i%3 !=1)
                    count++;
                if(j%3 !=1)
                    count++;
                count += std::abs(i/3-j/3);
                file << count;
            }
            if(j<GRAPH_SIZE-1)
                file << ",";
        }
        file << endl;
    }
    if(NUM_LINES-threads_used*INP_PER_THREAD<INP_PER_THREAD)
        file << NUM_LINES-threads_used*INP_PER_THREAD <<endl;
    else
        file << INP_PER_THREAD << endl;
    threads_used ++;
    file << k<<endl;
}
int generate_combinations(vector<int>& current, int length, ofstream& file, int gsize, int count, int mod) {
    //cout << count <<" of " << NUM_LINES << endl;
    if(count%mod == 0 && !printed){
        printMspace(file);
        printed = true;
    }
    else if(count%mod !=0)
        printed = false;
    if (current.size() == length) {
        for (int i = 0; i < length - 1; i++) {
            file << current[i] << ",";
        }
        file << current[length - 1] << endl;
        count ++;
        return count;
    }
    for (int i = 0; i < gsize; i++) {
        current.push_back(i);
        count = generate_combinations(current, length, file, gsize, count, mod);
        current.pop_back();
    }
    return count;
}

int main(int argc, char **argv) {
    GRAPH_SIZE = 3 * std::stoi(argv[1]);
    LINE_LENGTH = std::stoi(argv[2]);
    k = std::stoi(argv[3]);
    NUM_LINES = std::pow(GRAPH_SIZE, LINE_LENGTH);
    INP_PER_THREAD = NUM_LINES/NUM_THREADS;
    ofstream file;
    file.open("file.csv");
    file << "1,1,1,1,1,1" <<endl;
    if(NUM_LINES%NUM_THREADS == 0)
        file << NUM_THREADS << endl;
    else
        file << NUM_THREADS+1 <<endl;
    vector<int> current;
    generate_combinations(current, LINE_LENGTH, file, GRAPH_SIZE, 0, INP_PER_THREAD);
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