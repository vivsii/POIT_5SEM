#include "L4Thread.h"
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm> 

extern std::vector<int> global_primes;
extern std::mutex mtx;

void L4Thread(int start, int end) {
    std::vector<int> local_primes;


    for (int num = start; num <= end; ++num) {
        bool is_prime = true;
        if (num < 2) continue;
        for (int i = 2; i * i <= num; ++i) {
            if (num % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) local_primes.push_back(num);
    }

    std::lock_guard<std::mutex> lock(mtx);
    global_primes.insert(global_primes.end(), local_primes.begin(), local_primes.end());
}