#ifndef WORKER
#define WORKER

class Worker {
private:
    std::thread* thread;

public:
    Worker(std::thread* thread) {
        this->thread = thread;
    }

    ~Worker() {
        this->thread->join();
        delete this->thread;
    }
};

#endif // WORKER