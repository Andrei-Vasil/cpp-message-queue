#include <iostream>
#include <thread>
#include "queue.h"

int main() {
    Queue<int> Q = Queue<int>();
    Q.push(5);
    std::cout << Q.pop() << "\n";
    
    return 0;
}
