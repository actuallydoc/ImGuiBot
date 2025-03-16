//
// Created by Maj Knez on 16. 3. 25.
//

#include "Client.h"
#include <iostream>


Client::Client() {
    this->curl = curl_easy_init();
    if (!this->curl) {
        std::cerr << "Failed to initialize cURL" << std::endl;
    }
    this->url = std::string("http://127.0.0.1:8000/");
}

Client::~Client() {
    curl_global_cleanup();
    if (this->curl) {
        curl_easy_cleanup(this->curl);  // Clean up cURL once in the destructor
    }
    this->connected = false;
}
bool Client::connect() {
    if (this->curl) {
        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, this->GetURL(URL::KEEP_ALIVE).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(this->curl);
        if (res == CURLE_OK) {
            try {
                json jsonResponse = json::parse(response);
                if (jsonResponse["status"].is_boolean() == 1) {
                    bool status = jsonResponse["status"].get<bool>();
                    if (status == true) {
                        this->connection_status = Status::CONNECTED;
                    } else {
                        this->connection_status = Status::DISCONNECTED;
                    }
                }
            } catch (json::parse_error& e) {
                std::cerr << "JSON Parsing Error: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        }
    }
}
bool Client::GetServerStatus() {
    if (this->curl) {
        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, this->GetURL(URL::KEEP_ALIVE).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(this->curl);
        if (res == CURLE_OK) {
            try {
                json jsonResponse = json::parse(response);
                if (jsonResponse["status"].is_boolean() == 1) {
                    bool status = jsonResponse["status"].get<bool>();
                    if (status == true) {
                        this->connection_status = Status::CONNECTED;
                    } else {
                        this->connection_status = Status::DISCONNECTED;
                    }
                }
            } catch (json::parse_error& e) {
                this->connection_status = Status::DISCONNECTED;
                return false;
            }
        } else {
            this->connection_status = Status::DISCONNECTED;
            return false;
        }
    }
    return true;
}

std::string Client::GetURL(URL url) {
    switch (url) {
        case URL::KEEP_ALIVE:
            return this->url + "keep-alive";
        case URL::INDEX:
            return this->url;
    }
    return NULL;
}

std::string Client::GetStatus(Status status) {
    switch (status) {
        case Client::Status::CONNECTED:
            return "CONNECTED";
        case Client::Status::DISCONNECTED:
            return "DISCONNECTED";
        case Client::Status::RECONNECTING:
            return "RECONNECTING";
        case Client::Status::RECONNECTED:
            return "RECONNECTED";
        default:
            return "UNKNOWN";
    }
}

ImVec4 Client::getConnectionColor(Client::Status type) {
    switch (type) {
        case Client::Status::CONNECTED:
            return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);  // Green for connected
        case Client::Status::DISCONNECTED:
            return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);  // Red for disconnected
        case Client::Status::RECONNECTING:
            return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);  // Yellow for reconnecting
        case Client::Status::RECONNECTED:
            return ImVec4(0.0f, 1.0f, 1.0f, 1.0f);  // Cyan for successfully reconnected
        default:
            return ImVec4(0.5f, 0.5f, 0.5f, 1.0f);  // Gray for unknown state
    }
}


size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}
