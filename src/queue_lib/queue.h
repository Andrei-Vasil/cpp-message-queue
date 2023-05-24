#ifndef QUEUE
#define QUEUE

#include <queue>
#include <thread>
#include <condition_variable>
#include "../multithread_utilities_lib/worker.h"
#include "../multithread_utilities_lib/thread_complete_notifier.h"
#include "../benchmark_lib/benchmark.h"

template <typename T>
class Queue {
private:
    std::queue<T> q;
    std::mutex q_mutex = std::mutex();
    std::condition_variable q_condvar = std::condition_variable();
    std::vector<std::tuple<Worker*, ThreadCompleteNotifier<bool>*>> workers;
    std::tuple<Worker*, ThreadCompleteNotifier<bool>*> daemon;
    
    void low_level_push(T elem) {
        std::unique_lock q_lk(this->q_mutex);
        this->q.push(elem);
        this->q_condvar.notify_one();
    }

    void low_level_pop(ThreadCompleteNotifier<T>* notifier) {
        std::unique_lock q_lock(this->q_mutex);
        while (this->q.empty()) {
            this->q_condvar.wait(q_lock);
        }
        notifier->notify(this->q.front());
        this->q.pop();
    }

    void remove_workers() {
        for (auto worker_tuple_ptr = this->workers.begin(); worker_tuple_ptr < this->workers.end(); worker_tuple_ptr++) {
            auto worker_tuple = *worker_tuple_ptr;
            Worker* worker = std::get<0>(worker_tuple);
            ThreadCompleteNotifier<bool>* notifier = std::get<1>(worker_tuple);
            notifier->get();
            this->workers.erase(worker_tuple_ptr);
            delete worker;
            delete notifier;
        }
    }

    void start_daemon(ThreadCompleteNotifier<bool>* daemon_notifier) {
        while (true) {
            try {
                daemon_notifier->try_get_non_blocking();
                return;
            } catch (bool x) {}
            this->remove_workers();
        }
    }

public:
    Queue<T>() {
        this->q = std::queue<T>();
        this->workers = std::vector<std::tuple<Worker*, ThreadCompleteNotifier<bool>*>>();
        ThreadCompleteNotifier<bool>* daemon_notifier = new ThreadCompleteNotifier<bool>{};
        this->daemon = std::make_tuple(
            new Worker {
                new std::thread(&Queue::start_daemon, this, daemon_notifier),
            },
            daemon_notifier
        );
    }

    ~Queue<T>() {
        std::get<1>(this->daemon)->notify(true);
        delete std::get<0>(this->daemon);
        delete std::get<1>(this->daemon);
        this->remove_workers();
    }

    void push(T elem, int benchmark_id, std::string scenario_id) {
        ThreadCompleteNotifier<bool>* worker_notifier = new ThreadCompleteNotifier<bool>{};
        this->workers.push_back(std::make_tuple(
            new Worker {
                new std::thread([this, elem, worker_notifier, benchmark_id] {
                    this->low_level_push(elem);
                    set_publish_over(benchmark_id, scenario_id);
                    count_publish_throughput(scenario_id);
                    worker_notifier->notify(true);
                }),
            },
            worker_notifier
        ));
    }

    T pop(std::string scenario_id) {       
        ThreadCompleteNotifier<T>* notifier = new ThreadCompleteNotifier<T>{};

        ThreadCompleteNotifier<bool>* worker_notifier = new ThreadCompleteNotifier<bool>{};
        this->workers.push_back(std::make_tuple(
            new Worker {
                new std::thread([this, notifier, worker_notifier] {
                    this->low_level_pop(notifier);
                    count_consumer_throughput(scenario_id);
                    worker_notifier->notify(true);
                })
            },
            worker_notifier
        ));
        
        T val_to_return = notifier->get();
        delete notifier;
        return val_to_return;
    }
};

#endif // QUEUE
