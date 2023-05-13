//
//  Buffer.hpp
//  ProducerConsumer
//
//  Created by Andrew Wei on 5/31/21.
//

#ifndef Buffer_hpp
#define Buffer_hpp

#include <mutex>
#include <condition_variable>
#include <stdio.h>
#include "out.h"
 
#define BUFFER_CAPACITY 10

class Buffer {
    // Buffer fields
    Out buffer [BUFFER_CAPACITY];
    int buffer_size;
    int left; // index where variables are put inside of buffer (produced)
    int right; // index where variables are removed from buffer (consumed)
    
    // Fields for concurrency
    std::mutex mtx;
    std::condition_variable not_empty;
    std::condition_variable not_full;
    
public:
    // Place integer inside of buffer
    void produce(int thread_id, Out num);
    
    // Remove integer from buffer
    Out consume(int thread_id);
    
    Buffer();
};

#endif /* Buffer_hpp */