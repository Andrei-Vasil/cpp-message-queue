#include <iostream>
#include "queue.h"

int main() {
    queue<int> q;
    push(q, 1);
    std::cout << pop(q);
    return 0;
}
