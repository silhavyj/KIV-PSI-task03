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
        // e.g. GET /iss-now.json HTTP/1.1
        m_httpRequest = std::string(HTTP_GET) + " " + path + " " + HTTP_V1 + "\n";
    }

    void RestAPIClient::addHeader(const std::string &key, const std::string &value) {
        // e.g Host: api.open-notify.org
        m_httpRequest += key  + ": " + value + "\n";
    }

    std::string RestAPIClient::fetchData() {
        char buffer[BUFF_SIZE];
        int clientFd;
        std::string response;
        struct sockaddr_in server_address {};

        // There must be an empty line after the last header.
        m_httpRequest += "\n";

        // Create a socket.
        if ((clientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            std::cout << "ERR: could not initialize socket (" << m_ip << ":" << m_port << ")\n";
            return "";
        }

        // Initialize connection.
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(m_port);
        if (inet_pton(AF_INET, m_ip.c_str(), &server_address.sin_addr) <=0) {
            std::cout << "ERR: could not initialize connection\n";
            return "";
        }

        // Attempt to connect to the server.
        auto serverAddressPtr = reinterpret_cast<struct sockaddr *>(&server_address);
        auto serverAddressSize = sizeof(server_address);
        if (connect(clientFd, serverAddressPtr, serverAddressSize) < 0) {
            std::cout << "ERR: could not establish connection with to server\n";
            return "";
        }

        // Send the request off to the server.
        if (send(clientFd, m_httpRequest.c_str(), m_httpRequest.length(), 0) == -1) {
            std::cout << "ERR: failed to send data to the server\n";
            return "";
        }

        // Receive a response from the server.
        size_t readBytes = read(clientFd, buffer, BUFF_SIZE - 1);
        buffer[readBytes] = '\0';

        // Close up the socket.
        close(clientFd);

        // Fetch the data part off of the response (it's separated from the headers by HTTP_DATA_SEPARATOR).
        response = buffer;
        std::size_t position = response.find(HTTP_DATA_SEPARATOR);
        if (position == std::string::npos) {
            std::cout << "ERR: could not find data in the response from the server\n";
            return "";
        }

        // Trim off any unwanted data the JSON data may be surrounded with.
        std::string data = response.substr(position + strlen(HTTP_DATA_SEPARATOR));
        trimJSONData(data);
        return data;
    }

    void RestAPIClient::trimJSONData(std::string &data) {
        // Keep erasing the first character until you reach a '{'
        while (!data.empty() && data[0] != OPENING_CURLY) {
            data = data.erase(0, 1);
        }
        // Keep popping out the last character until you reach a '}'
        while (!data.empty() && data[data.length() - 1] != CLOSING_CURLY) {
            data.pop_back();
        }
    }
}