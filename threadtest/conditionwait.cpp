#include <iostream>                // std::cout
#include <thread>                // std::thread, std::this_thread::yield
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable
#include <vector>                // std::vector
using namespace std;
std::mutex mtx;
std::condition_variable cv;
 
int cargo = 0;
bool shipment_available()//cargo =0 return false; cargo != 0 return true 
{
    return cargo != 0;
}
 
// 消费者线程.
void consume(int id) {
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck, shipment_available);
    std::cout << "Thread " << id << " consumed: " << cargo << '\n';
    cargo = 0;
}
 
int main()
{
    vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        thread consumer_thread(consume, i);
        threads.push_back(std::move(consumer_thread));
    }
     // 创建十个消费者线程.
 
    // 主线程为生产者线程, 生产 10 个物品.
    for (int i = 0; i < 10; ++i) {
        while (shipment_available())
            std::this_thread::yield();
        std::unique_lock <std::mutex> lck(mtx);
        cargo = i + 1;
        cv.notify_one();
    }
 
    for(auto &thread : threads) {
        thread.join();
    }
 
    return 0;
}
