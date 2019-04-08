// Implementation inspired from:
// https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange

#include <iostream>
// std::atomic, std::atomic_flag
#include <atomic>

// std::thread, std::this_thread::yield
#include <thread>

#include <vector>
#include <cassert>

// constructing atomics
std::atomic_bool ready (false);

int MANY_TIME = 10000000; // 10 millions
int NB_THREAD = 5;
std::atomic_bool busy (false);
int shared_int = 0;

void count1m (int id) {
  // wait for the ready signal
  while (!ready){
    std::this_thread::yield();
  }

  for (int i = 0; i < MANY_TIME ; ++i) {
    bool initiallyFalse = false;
    while(!busy.compare_exchange_strong(initiallyFalse, true,
    					std::memory_order_relaxed)) {
      initiallyFalse = false;
    }
    ++shared_int;
    busy = false;
  }
};

int main ()
{
  std::vector<std::thread> threads;
  // Spawn 5 threads that count together to 5 * MANY_TIME
  for (int i = 1 ; i <= NB_THREAD ; ++i) {
    threads.push_back(std::thread(count1m,i));
  }
  ready = true;
  for (auto& th : threads)
    th.join();

  std::cout << "Finally shared_int: " << shared_int << std::endl;
  assert(shared_int == 5 * MANY_TIME);
  return 0;
}

// g++ -O1 -o atomic atomic.cpp -lpthread && ./atomic
