#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include "../cpp/RAII_Classes/writeOutput.cpp"
#include <cmath>
using namespace std;

int GRAPH_SIZE = 10;
int LINE_LENGTH = 6;
int NUM_LINES = 1000000;
int k = 3;
int NUM_THREADS = 1;
int INP_PER_THREAD = NUM_LINES/NUM_THREADS;
int threads_used = 0;
bool printed = false;
//generate a metric space with n legs. this will be a star graph (one vector at the middle, and a bunch of legs going off). All branches are of unit length (1), but the legs can have variable length. The legs can have length anywhere from 1 point to (n-1)/2 points. The metric space is returned as a 2d vector, where each location stores the distance from the row index to the column index (an nxn matrix).
std::vector<std::vector<int> > generate_Mspace(int n){
    //a vector to store the metric space
    std::vector<std::vector<int> > mspace;
    //a vector to store the leg on which each node lies. this will have the number of nodes as length, and each entry will be the leg number of that node.
    std::vector<int> leg;
    //the center node is on leg 0
    leg.push_back(0);
    // a vector to store the distance to the center for every node. This will have the number of nodes as length, and each entry will be the distance to the center of that node.
    std::vector<int> dist_to_center;
    //the center node is distance 0 from itself
    dist_to_center.push_back(0);
    //generate a random length for each leg. There will be n legs, and no leg can be longer than (n-1)/2, and must have length at least 1.
    for(int i = 1;i<n;i++){
        int length = rand()%(n/2);
        if(length == 0)
            length = 1;
        //for the number of nodes in this leg, add the leg number to the leg vector.
        for(int j = 0;j<length;j++){
            leg.push_back(i);
            //upade the dist_to_center vector. The distance to the center is the j+1.
            dist_to_center.push_back(j+1);
        }
    }
    //now, generate the metric space. For each node, find the distance to every other node. If we are on the same leg, the distance is the absolute value of the difference between the indeces of the nodes. If we are on different legs, the distance is the sum of the distance to the center of each node.
    for(int i = 0;i<n;i++){
        std::vector<int> row;
        for(int j = 0;j<n;j++){
            if(leg[i] == leg[j])
                row.push_back(abs(i-j));
            else
                row.push_back(dist_to_center[i]+dist_to_center[j]);
        }
        mspace.push_back(row);
    }


    return mspace;
}

void printMspace(ofstream& file){
    std::vector<std::vector<int> > mspace = generate_Mspace(GRAPH_SIZE);
    GRAPH_SIZE = mspace.size();
    
    
    file << GRAPH_SIZE << endl;
    //The ceter node is node 0, all other nodes are points.
    for(int i = 0;i<GRAPH_SIZE;i++){
        for(int j = 0;j<GRAPH_SIZE;j++){
            if(i == j)
                file << 0;
            else if(j == 0 || i ==0)
                file << 1;
            else
                file << 2;
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
    GRAPH_SIZE = std::stoi(argv[1]);
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