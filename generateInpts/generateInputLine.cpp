#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include "../cpp/RAII_Classes/writeOutput.cpp"
#include <cmath>
using namespace std;

int GRAPH_SIZE = 3;
int LINE_LENGTH = 3;
int NUM_LINES = 27;
int k = 2;
int NUM_THREADS = 1;
int INP_PER_THREAD = NUM_LINES/NUM_THREADS;
int threads_used = 0;
bool printed = false;

void printMspace(ofstream& file){
    file << GRAPH_SIZE << endl;
        for(int i = 0;i<GRAPH_SIZE;i++){
            for(int j = 0;j<GRAPH_SIZE;j++){
                int value = abs(j-i);
                file << value;
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
    cout << count <<" of " << NUM_LINES << endl;
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

int main() {
    ofstream file;
    file.open("file1.csv");
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
