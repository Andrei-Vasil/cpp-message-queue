#include <iostream>
#include "queue_lib/queue.h"

int main() {
    Queue<int>* Q = new Queue<int>();
    Q->push(5);
    std::cout << Q->pop() << "\n";    
    
    delete Q;

    return 0;
}
