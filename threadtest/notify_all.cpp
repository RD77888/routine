#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>
#include <chrono>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;
std::vector<std::string> shared_data; // 共享数据结构

// 使用普通 notify_all() 的工作线程
void worker_with_notify_all() {
    // 第一阶段 - 获取锁并通知
    {
        std::unique_lock<std::mutex> lck(mtx);
        std::cout << "[notify_all] 线程开始工作\n";
        ready = true;
        
        // 立即发送通知 - 此时其他线程虽被唤醒，但仍需等待锁释放
        cv.notify_all();
        std::cout << "[notify_all] 已通知等待线程，但锁尚未释放\n";
    } // 锁在此处释放，等待的线程此时可以获取锁并继续执行
    std::cout << "[notify_all] 锁已释放，等待线程现在可以执行\n";
    
    // 第二阶段 - 无锁状态下，继续执行自己的工作
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // 没有锁保护的数据访问（潜在的竞态条件）
    shared_data.push_back("notify_all数据 - 在通知后异步添加");
    std::cout << "[notify_all] 线程完成工作并退出\n";
}

// 使用 notify_all_at_thread_exit() 的工作线程
void worker_with_notify_at_exit() {
    std::unique_lock<std::mutex> lck(mtx);
    std::cout << "[notify_at_exit] 线程开始工作\n";
    ready = true;
    
    // 在进行通知前，先完成所有需要锁保护的操作
    shared_data.push_back("notify_at_exit数据 - 在通知前完成");
    std::cout << "[notify_at_exit] 数据准备完毕，即将转移锁所有权\n";
    
    // 注册线程退出时的通知行为，此调用后lck不再持有锁
    std::notify_all_at_thread_exit(cv, std::move(lck));
    
    // 以下操作不再有锁保护，但通知要等到整个函数执行完才发出
    std::cout << "[notify_at_exit] 锁所有权已转移，但通知尚未发出\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "[notify_at_exit] 线程即将退出，此时才会发出通知\n";
} // 线程结束时才会通知等待线程

// 等待线程函数
void waiter(int id, const std::string& type) {
    std::unique_lock<std::mutex> lck(mtx);
    std::cout << type << " 等待线程 " << id << " 开始等待\n";
    
    while (!ready) cv.wait(lck);
    
    // 被唤醒后立即记录状态
    std::cout << type << " 等待线程 " << id << " 被唤醒\n";
    
    // 访问共享数据
    if (!shared_data.empty()) {
        std::cout << type << " 等待线程 " << id << " 读取数据: " 
                 << shared_data.back() << "\n";
    } else {
        std::cout << type << " 等待线程 " << id << " 发现共享数据为空!\n";
    }
}
int main() {
    // 测试 notify_all()
    {
        ready = false;
        shared_data.clear();
        
        std::cout << "\n===== 测试 notify_all() =====\n\n";
        
        std::vector<std::thread> waiters;
        for (int i = 0; i < 3; ++i) {
            waiters.emplace_back(waiter, i, "notify_all");
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::thread worker(worker_with_notify_all);
        
        for (auto& t : waiters) t.join();
        worker.join();
    }
    
    // 测试 notify_all_at_thread_exit()
    {
        ready = false;
        shared_data.clear();
        
        std::cout << "\n===== 测试 notify_all_at_thread_exit() =====\n\n";
        
        std::vector<std::thread> waiters;
        for (int i = 0; i < 3; ++i) {
            waiters.emplace_back(waiter, i, "notify_at_exit");
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::thread worker(worker_with_notify_at_exit);
        
        for (auto& t : waiters) t.join();
        worker.join();
    }
    
    return 0;
}