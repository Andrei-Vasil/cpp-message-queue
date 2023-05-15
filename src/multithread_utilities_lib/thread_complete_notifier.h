#ifndef THREAD_COMPLETE_NOTIFIER
#define THREAD_COMPLETE_NOTIFIER

template <typename T> 
class ThreadCompleteNotifier {
private:
    std::condition_variable condvar();
    std::mutex mutex();
    T return_val();
    bool returned = false;

public:
    ThreadCompleteNotifier() {}

    void notify(T return_val = T()) {
        std::unique_lock(this->mutex);
        this->return_val = return_val;
        this->returned = true;
        this->condvar.notify_one();
    }

    T get() {
        auto lock = std::unique_lock(this->mutex);
        while (!this->returned) {
            this->condvar.wait(lock);
        }
        return this->return_val;
    }

    T try_get_non_blocking() {
        auto lock = std::unique_lock(this->mutex);
        if (!this->returned) {
            throw false;
        }
        return this->return_val;
    }

    ~ThreadCompleteNotifier() {}
};

#endif // THREAD_COMPLETE_NOTIFIER