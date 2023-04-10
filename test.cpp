#include<vector>
#include <iostream>
int main() {
    if(true){
        std::vector<int> bad_reference;
        std::vector<int> good_reference;
        bad_reference.reserve(5);
        good_reference.reserve(5);
        bad_reference[0] = 1;
        good_reference.push_back(1);
        std::cout << "Both of these print out 1: \n";
        std::cout <<bad_reference[0] << "\n";
        std::cout << good_reference[0] << "\n";
        //We can even change the value at the specified location once it has been assigned
        good_reference[0] = 2;

    }
    //Once we go out of scope, there is a memory leak from the
    //bad_reference vector
    return 0;
}