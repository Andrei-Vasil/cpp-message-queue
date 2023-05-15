#ifndef SUBSCRIPTION_MANAGER
#define SUBSCRIPTION_MANAGER

class SubscriptionManager {
private:
    QueueManager* queue_manager;

public:
    SubscriptionManager(QueueManager* queue_manager) {
        this->queue_manager = queue_manager;
    }
};

#endif // SUBSCRIPTION_MANAGER