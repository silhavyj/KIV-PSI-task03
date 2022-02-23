#pragma once

#include <string>
#include <cstdint>

namespace psi {

    class RestAPIClient {
    private:
        static constexpr uint32_t BUFF_SIZE = 1024;
        static constexpr const char *HTTP_GET = "GET";
        static constexpr const char *HTTP_V1 = "HTTP/1.1";
        static constexpr const char *HTTP_DATA_SEPARATOR = "\r\n\r\n";
        static constexpr char OPENING_CURLY = '{';
        static constexpr char CLOSING_CURLY = '}';

        std::string m_httpRequest;
        std::string m_ip;
        uint16_t m_port;

    private:
        static void trimJSONData(std::string &data);

    public:
        RestAPIClient(std::string ip, uint16_t port);
        std::string fetchData();
        void createGETRequest(const std::string &path);
        void addHeader(const std::string &key, const std::string &value);
    };
}