#ifndef HTPP_REQUEST_HANDLER
#define HTPP_REQUEST_HANDLER

#include "../../external_libs/cpp-httplib/httplib.h"
#include "../../external_libs/json/single_include/nlohmann/json.hpp"
#include "../string_utilities_lib/string_utilities.h"

using json = nlohmann::json;

class HttpRequestHandler {

private:
    httplib::Server server;

    std::vector<std::string> extract_path_parameters(const httplib::Request &request) {
        std::vector<std::string> parameters = split(request.path, "/");
        return std::vector<std::string>(parameters.begin() + 2, parameters.end());
    }

    std::string new_topic(std::vector<std::string> parameters) {
        std::ostringstream oss; 
        if (parameters.size() != 1) {
            oss << "HTTP/1.1 404 NOT FOUND\r\n\r\nInvalid path\r\n";
            return oss.str();
        }
        std::string topic = parameters[0];
        // TODO: connect with backend
        oss << "New topic" << "\r\n";
        return oss.str();
    }

    std::string remove_topic(std::vector<std::string> parameters) {
        std::ostringstream oss; 
        if (parameters.size() != 1) {
            oss << "HTTP/1.1 404 NOT FOUND\r\n\r\nInvalid path\r\n";
            return oss.str();
        }
        std::string topic = parameters[0];
        // TODO: connect with backend
        oss << "Remove topic" << "\r\n";
        return oss.str();
    }

    std::string subscribe(std::vector<std::string> parameters) {
        std::ostringstream oss; 
        if (parameters.size() != 1) {
            oss << "HTTP/1.1 404 NOT FOUND\r\n\r\nInvalid path\r\n";
            return oss.str();
        }
        std::string topic = parameters[0];
        // TODO: connect with backend
        oss << "Subscribe" << "\r\n";
        return oss.str();
    }

    std::string unsubscribe(std::vector<std::string> parameters) {
        std::ostringstream oss; 
        if (parameters.size() != 2) {
            oss << "HTTP/1.1 404 NOT FOUND\r\n\r\nInvalid path\r\n";
            return oss.str();
        }
        std::string topic = parameters[0];
        int id = atoi(parameters[1].c_str());
        // TODO: connect with backend
        oss << "Unsubscribe" << "\r\n";
        return oss.str();
    }

    std::string publish(std::vector<std::string> parameters, std::string body) {
        std::ostringstream oss; 
        if (parameters.size() != 1) {
            oss << "HTTP/1.1 404 NOT FOUND\r\n\r\nInvalid path\r\n";
            return oss.str();
        }
        std::string topic = parameters[0];
        int message = json::parse(body)["item"];
        // TODO: connect with backend
        oss << "Publish" << "\r\n";
        return oss.str();
    }

    std::string retrieve(std::vector<std::string> parameters) {
        std::ostringstream oss; 
        if (parameters.size() != 2) {
            oss << "HTTP/1.1 404 NOT FOUND\r\n\r\nInvalid path\r\n";
            return oss.str();
        }
        std::string topic = parameters[0];
        int id = atoi(parameters[1].c_str());
        // TODO: connect with backend
        oss << "Retrieve" << "\r\n";
        return oss.str();
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

    HttpRequestHandler() {}

    void run() {
        this->setup();
        this->server.listen("0.0.0.0", 5000);
    }

    ~HttpRequestHandler() {}

};

#endif // HTPP_REQUEST_HANDLER
