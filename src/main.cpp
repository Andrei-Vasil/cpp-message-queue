#include <iostream>
#include "queue_lib/queue.h"
#include "http_comms_lib/http_request_handler.h"
#include "pub_sub_lib/shared_memory.h"
#include "pub_sub_lib/topic_manager.h"
#include "pub_sub_lib/subscription_manager.h"
#include "pub_sub_lib/queue_manager.h"
#include "benchmark_lib/benchmark.h"

bool BenchmarkingMetrics::BENCHMARKING = false;

#include <cstdio>
#include <ctime>

int main() {
    BenchmarkingMetrics::init_vars();

    SharedMemory* shared_memory = new SharedMemory{};
    TopicManager* topic_manager = new TopicManager{shared_memory};
    QueueManager* queue_manager = new QueueManager{shared_memory, topic_manager};
    SubscriptionManager* subscription_manager = new SubscriptionManager{queue_manager};

    HttpRequestHandler* http_request_handler = new HttpRequestHandler{topic_manager, queue_manager, subscription_manager};
    http_request_handler->run();

    delete http_request_handler;
    delete subscription_manager;
    delete queue_manager;
    delete topic_manager;
    delete shared_memory;

    return 0;
}
