#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

int GRAPH_SIZE = 10;
int LINE_LENGTH = 20;
int NUM_LINES = 10000000;
int k = 3;
int NUM_THREADS = 16;
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

int main() {
    ofstream file;
    file.open("file.csv");
    file << "1,1,1,1,1,1" <<endl;
    if(NUM_LINES%NUM_THREADS == 0)
        file << NUM_THREADS << endl;
    else
        file << NUM_THREADS+1 <<endl;
    

    srand(time(NULL));
    for (int i = 0; i < NUM_LINES; i++) {
        if(i%INP_PER_THREAD == 0)
            printMspace(file);
        for (int j = 0; j < LINE_LENGTH-1; j++) {
            int digit = rand() % 10;
            file << digit<< ',';
        }
        int digit = rand()%10;
        file << digit << endl;
    }
    file.close();
    return 0;
}