#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class Queue {
private:
    std::queue<T> q;
    std::vector<std::thread*> threads;
    std::mutex q_mutex;
    
    void low_level_push(T elem) {
        std::cout << "pushin fr\n";
        std::unique_lock q_lk(q_mutex);
        q.push(elem);
        q_lk.unlock();
    }

    void low_level_pop(std::condition_variable* condvar, std::mutex* mutex, T* return_val_ptr, bool* returned) {
        std::cout << "poppin ong\n";

        std::unique_lock lk(*mutex);
        std::unique_lock q_lk(q_mutex);
        *return_val_ptr = T(q.front());
        *returned = true;
        q.pop();
        lk.unlock();
        q_lk.unlock();
        condvar->notify_one();
    }

public:
    Queue<T>() {
        q = std::queue<T>();
        threads = std::vector<std::thread*>();
    }

    void push(T elem) {
        std::thread* worker = new std::thread(&Queue<T>::low_level_push, this, elem);
        threads.push_back(worker);
    }

    T pop() {
        bool* returned = new bool(false);
        T* return_val_ptr = new T();
        std::condition_variable* condvar = new std::condition_variable();
        std::mutex* condvar_mutex = new std::mutex();
        std::thread* worker = new std::thread(&Queue<T>::low_level_pop, this, condvar, condvar_mutex, return_val_ptr, returned);
        threads.push_back(worker);
        
        std::unique_lock condvar_lk(*condvar_mutex);
        while (!*returned) {
            condvar->wait(condvar_lk);
        }
        condvar_lk.unlock();
        T return_val = *return_val_ptr;
        delete return_val_ptr;
        delete returned;
        delete condvar;
        delete condvar_mutex;
        return return_val;
    }
};
