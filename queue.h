#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
struct queue {
    std::queue<T> q;
    std::vector<std::thread> threads;
};

namespace details {
    template <typename T>
    void low_level_push(queue<T> queue, T elem) {
        std::cout << "pushin fr\n";
    }

    template <typename T>
    T low_level_pop(queue<T> queue) {
        std::cout << "poppin fr\n";
        return T();
    }
}

template <typename T>
void push(queue<T> queue, T elem) {
    details::low_level_push(queue, elem);
}

template <typename T>
T pop(queue<T> queue) {
    return details::low_level_pop(queue);
}
