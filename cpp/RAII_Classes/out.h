//author: Stefan Caldararu
//output object, just a struct.

#ifndef OUT_H
#define OUT_H
#include <vector>

struct Out
{
    std::vector<int> input;
    int rand;
    int greedy;
    int OPT;
    int WFA;
    int DC;
    int LDC;
    int KC;
};

#endif