#ifndef SUBSCRIPTION_MANAGER
#define SUBSCRIPTION_MANAGER

class SubscriptionManager {
private:
    QueueManager* queue_manager;

public:
    SubscriptionManager(QueueManager* queue_manager) {
        this->queue_manager = queue_manager;
    }

    std::string subscribe(std::string topic) {
        return this->queue_manager->create_queue_channel(topic);
    }

    std::string unsubscribe(std::string topic, int id) {
        return this->queue_manager->remove_queue_channel(topic, id);
    }
};

#endif // SUBSCRIPTION_MANAGER
