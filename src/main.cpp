#include <iostream>
#include "queue_lib/queue.h"
#include "http_comms_lib/http_request_handler.h"

int main() {
    // Queue<int>* Q = new Queue<int>();
    // Q->push(5);
    // std::cout << Q->pop() << "\n";        
    // delete Q;

    HttpRequestHandler httpRequestHandler = HttpRequestHandler{};
    httpRequestHandler.run();

    return 0;
}
