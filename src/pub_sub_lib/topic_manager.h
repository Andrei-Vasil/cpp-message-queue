#ifndef TOPIC_MANAGER
#define TOPIC_MANAGER

#include "shared_memory.h"

class TopicManager {
private:
    SharedMemory* shared_memory;

public:
    TopicManager(SharedMemory* shared_memory) {
        this->shared_memory = shared_memory;
    }

    std::string new_topic(std::string topic) {
        std::ostringstream oss; 
        if (this->exists(topic)) {
            oss << "Topic named " << topic << " already exists\r\n";
            return oss.str();    
        }
        this->shared_memory->add_topic(topic);
        this->shared_memory->put_max_id_4_topic(topic, 0);
        this->shared_memory->put_new_queue_channels_4_topic(topic);
        oss << "Successfully created the new topic " << topic << "\r\n";
        return oss.str();
    }

    std::string remove_topic(std::string topic) {
        std::ostringstream oss; 
        if (!this->exists(topic)) {
            oss << "There is no topic named: " << topic << "\r\n";
            return oss.str();    
        }
        this->shared_memory->remove_topic(topic);
        this->shared_memory->remove_max_id_4_topic(topic);
        this->shared_memory->remove_queue_channels_4_topic(topic);
        oss << "Successfully deleted " << topic << " topic\r\n";
        return oss.str();
    }

    bool exists(std::string topic) {
        return this->shared_memory->topic_exists(topic);
    }
};

#endif // TOPIC_MANAGER
