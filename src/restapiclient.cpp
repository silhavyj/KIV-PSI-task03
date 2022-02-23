#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <utility>

#include "restapiclient.h"

namespace psi {

    RestAPIClient::RestAPIClient(std::string ip, uint16_t port) : m_ip(std::move(ip)), m_port(port) {
    }

    void RestAPIClient::createGETRequest(const std::string &path) {
        m_httpRequest = std::string(HTTP_GET) + " " + path + " " + HTTP_V1 + "\n";
    }

    void RestAPIClient::addHeader(const std::string &key, const std::string &value) {
        m_httpRequest += key  + ": " + value + "\n";
    }

    std::string RestAPIClient::fetchData() {
        char buffer[BUFF_SIZE];
        int clientFd;
        std::string response;
        struct sockaddr_in server_address {};

        m_httpRequest += "\n";

        if ((clientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            std::cout << "ERR: could not initialize socket (" << m_ip << ":" << m_port << ")\n";
            return "";
        }

        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(m_port);
        if (inet_pton(AF_INET, m_ip.c_str(), &server_address.sin_addr) <=0) {
            std::cout << "ERR: could not initialize connection\n";
            return "";
        }

        auto serverAddressPtr = reinterpret_cast<struct sockaddr *>(&server_address);
        auto serverAddressSize = sizeof(server_address);
        if (connect(clientFd, serverAddressPtr, serverAddressSize) < 0) {
            std::cout << "ERR: could not establish connection with to server\n";
            return "";
        }

        if (send(clientFd, m_httpRequest.c_str(), m_httpRequest.length(), 0) == -1) {
            std::cout << "ERR: failed to send data to the server\n";
            return "";
        }

        size_t readBytes = read(clientFd, buffer, BUFF_SIZE - 1);
        buffer[readBytes] = '\0';
        close(clientFd);

        response = buffer;
        std::size_t position = response.find(HTTP_DATA_SEPARATOR);
        if (position == std::string::npos) {
            std::cout << "ERR: could not find data in the response from the server\n";
            return "";
        }

        std::string data = response.substr(position + strlen(HTTP_DATA_SEPARATOR));
        trimJSONData(data);
        return data;
    }

    void RestAPIClient::trimJSONData(std::string &data) {
        while (!data.empty() && data[0] != OPENING_CURLY) {
            data = data.erase(0, 1);
        }
        while (!data.empty() && data[data.length() - 1] != CLOSING_CURLY) {
            data.pop_back();
        }
    }
}