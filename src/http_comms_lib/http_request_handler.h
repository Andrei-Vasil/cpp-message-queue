#ifndef HTPP_REQUEST_HANDLER
#define HTPP_REQUEST_HANDLER

#include "../../external_libs/cpp-httplib/httplib.h"
#include "../../external_libs/json/single_include/nlohmann/json.hpp"
#include "../string_utilities_lib/string_utilities.h"
#include "../pub_sub_lib/topic_manager.h"
#include "../pub_sub_lib/queue_manager.h"
#include "../pub_sub_lib/subscription_manager.h"
#include "../pub_sub_lib/shared_memory.h"

using json = nlohmann::json;

class HttpRequestHandler {

private:
    TopicManager* topic_manager;
    QueueManager* queue_manager;
    SubscriptionManager* subscription_manager;
    httplib::Server server;

    std::vector<std::string> extract_path_parameters(const httplib::Request &request) {
        std::vector<std::string> parameters = split(request.path, "/");
        return std::vector<std::string>(parameters.begin() + 2, parameters.end());
    }

    std::string new_topic(std::vector<std::string> parameters) {
        if (parameters.size() != 1) {
            return "HTTP/1.1 404 NOT FOUND\r\n\r\nInvalid path\r\n";
        }
        std::string topic = parameters[0];
        return this->topic_manager->new_topic(topic);
    }

    std::string remove_topic(std::vector<std::string> parameters) {
        if (parameters.size() != 1) {
            return "HTTP/1.1 404 NOT FOUND\r\n\r\nInvalid path\r\n";
        }
        std::string topic = parameters[0];
        return this->topic_manager->remove_topic(topic);
    }

    std::string subscribe(std::vector<std::string> parameters) {
        if (parameters.size() != 1) {
            return "HTTP/1.1 404 NOT FOUND\r\n\r\nInvalid path\r\n";
        }
        std::string topic = parameters[0];
        return this->subscription_manager->subscribe(topic);
    }

    std::string unsubscribe(std::vector<std::string> parameters) {
        if (parameters.size() != 2) {
            return "HTTP/1.1 404 NOT FOUND\r\n\r\nInvalid path\r\n";
        }
        std::string topic = parameters[0];
        int id = atoi(parameters[1].c_str());
        return this->subscription_manager->unsubscribe(topic, id);
    }

    std::string publish(std::vector<std::string> parameters, std::string body) {
        if (parameters.size() != 1) {
            return "HTTP/1.1 404 NOT FOUND\r\n\r\nInvalid path\r\n";
        }
        std::string topic = parameters[0];
        std::string message = json::parse(body)["item"];
        int benchmark_id = json::parse(body)["benchmark_id"];
        return this->queue_manager->publish(topic, message, benchmark_id);
    }

    std::string retrieve(std::vector<std::string> parameters) {
        if (parameters.size() != 2) {
            return "HTTP/1.1 404 NOT FOUND\r\n\r\nInvalid path\r\n";
        }
        std::string topic = parameters[0];
        int id = atoi(parameters[1].c_str());
        return this->queue_manager->retrieve(topic, id);
    }

    void setup() {
        this->server.Post(R"(/topic/[^\/]+)", [this](const httplib::Request& request, httplib::Response& response, const httplib::ContentReader& content_reader) {
            std::vector<std::string> parameters = this->extract_path_parameters(request);
            std::string response_as_str = this->new_topic(parameters); 
            response.set_content(response_as_str, "text/plain");
        });

        this->server.Delete(R"(/topic/[^\/]+)", [this](const httplib::Request &request, httplib::Response &response) {
            std::vector<std::string> parameters = this->extract_path_parameters(request);
            std::string response_as_str = this->remove_topic(parameters); 
            response.set_content(response_as_str, "text/plain");
        });

        this->server.Post(R"(/subscription/[^\/]+)", [this](const httplib::Request &request, httplib::Response &response, const httplib::ContentReader& content_reader) {
            std::vector<std::string> parameters = this->extract_path_parameters(request);
            std::string response_as_str = this->subscribe(parameters); 
            response.set_content(response_as_str, "text/plain");
        });

        this->server.Delete(R"(/subscription/[^\/]+/[^\/]+)", [this](const httplib::Request &request, httplib::Response &response) {
            std::vector<std::string> parameters = this->extract_path_parameters(request);
            std::string response_as_str = this->unsubscribe(parameters); 
            response.set_content(response_as_str, "text/plain");
        });

        this->server.Post(R"(/publish/[^\/]+)", [this](const httplib::Request &request, httplib::Response &response, const httplib::ContentReader& content_reader) {
            std::vector<std::string> parameters = this->extract_path_parameters(request);
            std::string body;
            content_reader([&](const char *data, size_t data_length) {
                body.append(data, data_length);
                return true;
            });
            std::string response_as_str = this->publish(parameters, body); 
            response.set_content(response_as_str, "text/plain");
        });

        this->server.Get(R"(/subscription/[^\/]+/[^\/]+)", [this](const httplib::Request &request, httplib::Response &response) {
            std::vector<std::string> parameters = this->extract_path_parameters(request);
            std::string response_as_str = this->retrieve(parameters); 
            response.set_content(response_as_str, "text/plain");
        });
    }

public:

    HttpRequestHandler(TopicManager* topic_manager, QueueManager* queue_manager, SubscriptionManager* subscription_manager) {
        this->topic_manager = topic_manager;
        this->queue_manager = queue_manager;
        this->subscription_manager = subscription_manager;
    }

    void run() {
        this->setup();
        this->server.listen("0.0.0.0", 5000);
    }

    ~HttpRequestHandler() {}

};

#endif // HTPP_REQUEST_HANDLER
