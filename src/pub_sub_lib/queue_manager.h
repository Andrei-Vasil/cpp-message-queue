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

    std::string create_queue_channel(std::string topic) {
        std::ostringstream oss; 
        if (!this->topic_manager->exists(topic)) {
            oss << "There is no topic named: " << topic << "\r\n";
            return oss.str();
        }
        int id = this->shared_memory->get_max_id_4_topic(topic);
        this->shared_memory->put_max_id_4_topic(topic, id + 1);
        this->shared_memory->put_new_queue_channel_4_id(topic, id);
        oss << id << "\r\n";
        return oss.str();
    }

    std::string remove_queue_channel(std::string topic, int id) {
        std::ostringstream oss; 
        if (!this->topic_manager->exists(topic)) {
            oss << "There is no topic named: " << topic << "\r\n";
            return oss.str();
        }
        if (!this->shared_memory->queue_channel_exists_4_id(topic, id)) {
            oss << "There is no id with specified value: " << id << "\r\n";
            return oss.str();
        }
        this->shared_memory->remove_queue_channel_4_id(topic, id);
        oss << "Successfully unsubscribed id " << id << " from " << topic << " topic\r\n";
        return oss.str();
    }

    std::string publish(std::string topic, std::string message, int benchmark_id, std::string scenario_id) {
        std::ostringstream oss; 
        if (!this->topic_manager->exists(topic)) {
            oss << "There is no topic named: " << topic << "\r\n";
            return oss.str();
        }
        std::unique_lock(this->shared_memory->queue_channels_mutex);
        for (auto pair_ : this->shared_memory->queue_channels[topic]) {
            Queue<std::string>* queue = pair_.second;
            queue->push(message, benchmark_id, scenario_id);
        }
        oss << "Successfully published your message to " << topic << " topic\r\n";
        return oss.str();
    }

    std::string retrieve(std::string topic, int id, std::string scenario_id) {
        std::ostringstream oss; 
        if (!this->topic_manager->exists(topic)) {
            oss << "There is no topic named: " << topic << "\r\n";
            return oss.str();
        }
        if (!this->shared_memory->queue_channel_exists_4_id(topic, id)) {
            oss << "There is no id with specified value: " << id << "\r\n";
            return oss.str();
        }
        std::unique_lock(this->shared_memory->queue_channels_mutex);
        Queue<std::string>* queue = this->shared_memory->queue_channels[topic][id];
        oss << queue->pop(scenario_id) << "\r\n";
        return oss.str();
    }
};

#endif // QUEUE_MANAGER
