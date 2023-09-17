//
//  main.cpp
//  ProducerConsumer
//
//  Created by Andrew Wei on 5/30/21.
//

#include <thread>
#include <iostream>
#include "RAII_Classes/buffer.hpp"
#include <stdlib.h>
#include "RAII_Classes/out.h"

// Takes in reference to a buffer and adds a random integer
void produceInt(int x, Buffer &buffer) {
    for (int i = 0; i < 100; i++) {
        // Generate random number between 1 and 10
        int new_int = rand() % 10 + 1;
        Out o;
        o.rand = 1;
        buffer.produce(x, o);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// Takes in reference to a buffer and returns the latest int added
// in the buffer
void consumeInt(int x, Buffer &buffer) {
    for (int i = 0; i < 300; i++) {
        buffer.consume(x);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(int argc, const char * argv[]) {
    std::cout << "Executing code in main...\n";
    
    // Initialize random seed
    srand (time(NULL));
    
    // Create Buffer
    Buffer buffer;
    
    //int x = 1;
    // Create a thread to produce
    std::thread produceThread0(produceInt, 0, std::ref(buffer));
    
    std::thread produceThread1(produceInt, 1, std::ref(buffer));

    std::thread consumeThread0(consumeInt, 2, std::ref(buffer));
    
    // std::thread consumeThread1(consumeInt, 3, std::ref(buffer));
    
    std::thread produceThread2(produceInt, 4, std::ref(buffer));
    
    produceThread0.join();
    produceThread1.join();
    produceThread2.join();
    consumeThread0.join();
    // consumeThread1.join();
    
    std::cout << "Done!\n";
    return 0;
}