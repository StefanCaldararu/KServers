#include<iostream>
#include <omp.h>

int main()
{   
    #pragma omp parallel for num_threads(4)
    for(int i =0;i<4;i++){
        for(int j = 0;j<10000;j++){
            std::cout << "ran for input "<< j << " with thread_ID: " <<  << std::endl;
        }
    }
    return 0;   
}