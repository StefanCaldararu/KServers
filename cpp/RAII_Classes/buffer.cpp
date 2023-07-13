//
//  Buffer.cpp
//  ProducerConsumer
//
//  Created by Andrew Wei on 5/31/21.
//

#include <iostream>
#include "buffer.hpp"
#include "out.h"

Buffer::Buffer() {
    buffer_size = 0;
    left = 0;
    right = 0;
}

void Buffer::produce(int thread_id, Out num) {
    // Acquire a unique lock on the mutex
    std::unique_lock<std::mutex> unique_lock(mtx);
    
    //std::cout << "thread " << thread_id << " produced " << num.rand << "\n";
    
    // Wait if the buffer is full
    not_full.wait(unique_lock, [this]() {
        return buffer_size != BUFFER_CAPACITY;
    });
    
    // Add input to buffer
    buffer[right] = num;
    
    // Update appropriate fields
    right = (right + 1) % BUFFER_CAPACITY;
    buffer_size++;
    
    // Unlock unique lock
    unique_lock.unlock();
    
    // Notify a single thread that buffer isn't empty
    not_empty.notify_one();
}

Out Buffer::consume(int thread_id) {
    // Acquire a unique lock on the mutex
    std::unique_lock<std::mutex> unique_lock(mtx);
    // Wait if buffer is empty
    if(buffer_size == 0){
    not_empty.wait(unique_lock, [this]() {
        return buffer_size != 0;
    });
    }
    // std::cout << "left: " << left << " right: " << right << std::endl;
    // std::cout << "size: " << buffer_size << std::endl;
    // std::cout << "write2: " << thread_id << std::endl;
    
    // Getvalue from position to remove in buffer
    Out result = buffer[left];
    
    //std::cout << "thread " << thread_id << " consumed " << result.rand << "\n";
    
    // Update appropriate fields
    left = (left + 1) % BUFFER_CAPACITY;
    buffer_size--;
    
    // Unlock unique lock
    unique_lock.unlock();
    
    // Notify a single thread that the buffer isn't full
    not_full.notify_one();
    
    // Return result
    return result;
}