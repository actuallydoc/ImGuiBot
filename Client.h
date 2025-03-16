//
// Created by Maj Knez on 16. 3. 25.
//

#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <ImGUI/imgui.h>

// for convenience
using json = nlohmann::json;

class Client {

private:
    std::string url = std::string("http://127.0.0.1:8000/");

    CURL *curl;
public:
    bool connected = false;
    enum class URL {
        KEEP_ALIVE,
        INDEX,
    };

    enum class Status {
        CONNECTED,
        DISCONNECTED,
        RECONNECTED,
        RECONNECTING,
    };
    Status connection_status = Status::DISCONNECTED;
    // Constructor
    Client();
    // Destructor
    ~Client();

    bool connect();

    bool GetServerStatus();
    std::string GetURL(URL url);

    std::string GetStatus(Status status);

    ImVec4 getConnectionColor(Client::Status type);
};

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output);

#endif //CLIENT_H
