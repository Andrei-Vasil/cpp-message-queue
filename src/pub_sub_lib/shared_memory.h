#ifndef SHARED_MEMORY
#define SHARED_MEMORY

#include "../queue_lib/queue.h"

class SharedMemory {
public:
    std::set<std::string> topics = std::set<std::string>();
    std::mutex topics_mutex = std::mutex();
    std::unordered_map<std::string, std::unordered_map<int, Queue<int>*>> queue_channels = std::unordered_map<std::string, std::unordered_map<int, Queue<int>*>>();
    std::mutex queue_channels_mutex = std::mutex();
    std::unordered_map<std::string, int> max_id_4_topic = std::unordered_map<std::string, int>();
    std::mutex max_id_4_topic_mutex = std::mutex();

    bool topic_exists(std::string topic) {
        std::unique_lock(this->topics_mutex);
        return this->topics.find(topic) != this->topics.end();
    }

    void add_topic(std::string topic) {
        std::unique_lock(this->topics_mutex);
        this->topics.insert(topic);
    }

    void remove_topic(std::string topic) {
        std::unique_lock(this->topics_mutex);
        this->topics.erase(topic);
    }

    void put_max_id_4_topic(std::string topic, int id) {
        std::unique_lock(this->max_id_4_topic_mutex);
        this->max_id_4_topic[topic] = id;
    }

    void remove_max_id_4_topic(std::string topic) {
        std::unique_lock(this->max_id_4_topic_mutex);
        this->max_id_4_topic.erase(topic);
    }

    void put_new_queue_channels_4_topic(std::string topic) {
        std::unique_lock(this->queue_channels_mutex);
        this->queue_channels[topic] = std::unordered_map<int, Queue<int>*>();
    }

    void remove_queue_channels_4_topic(std::string topic) {
        std::unique_lock(this->queue_channels_mutex);
        this->queue_channels.erase(topic);
    }
};

#endif // SHARED_MEMORY
