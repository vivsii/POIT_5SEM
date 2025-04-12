#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm> 
#include <string>    
#include "L4Thread.h"

std::vector<int> global_primes;
std::mutex mtx;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: Lab-04 <num_threads> <lower_bound> <upper_bound>\n";
        return 1;
    }

    int num_threads = std::stoi(argv[1]);
    int lower_bound = std::stoi(argv[2]);
    int upper_bound = std::stoi(argv[3]);

    int range = (upper_bound - lower_bound + 1) / num_threads;
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        int start = lower_bound + i * range;
        int end = (i == num_threads - 1) ? upper_bound : start + range - 1;
        threads.emplace_back(L4Thread, start, end);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::sort(global_primes.begin(), global_primes.end());
    for (int prime : global_primes) {
        std::cout << prime << " ";
    }
    std::cout << std::endl;

    return 0;
}