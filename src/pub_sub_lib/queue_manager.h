#ifndef QUEUE_MANAGER
#define QUEUE_MANAGER

class QueueManager {
private:
    SharedMemory* shared_memory;
    TopicManager* topic_manager;

public:
    QueueManager(SharedMemory* shared_memory, TopicManager* topic_manager) {
        this->shared_memory = shared_memory;
        this->topic_manager = topic_manager;
    }
};

#endif // QUEUE_MANAGER