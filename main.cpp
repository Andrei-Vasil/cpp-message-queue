#include <iostream>
#include "queue.h"

int main() {
    Queue<int>* Q = new Queue<int>();
    Q->push(5);
    std::cout << Q->pop() << "\n";    
    
    delete Q;

    return 0;
}
