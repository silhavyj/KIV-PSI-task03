#pragma once

#include <string>
#include <cstdint>

namespace psi {

    /// This class is an implementation of a simple REST API client.
    /// It only supports the GET method as no other method is needed
    /// within this project.
    class RestAPIClient {
    private:
        /// Size of the buffer reserved for the response from the server.
        static constexpr uint32_t BUFF_SIZE = 1024;

        /// HTTP GET method
        static constexpr const char *HTTP_GET = "GET";

        /// Version of the HTTP protocol
        static constexpr const char *HTTP_V1 = "HTTP/1.1";

        /// Deliminator of the header and the data in an HTTP response.
        static constexpr const char *HTTP_DATA_SEPARATOR = "\r\n\r\n";

        /// Start of JSON data
        static constexpr char OPENING_CURLY = '{';

        /// End of JSON data
        static constexpr char CLOSING_CURLY = '}';

        /// HTTP GET request sent to the server
        std::string m_httpRequest;

        /// IP address of the server providing the API
        std::string m_ip;

        /// Port the API service is running on
        uint16_t m_port;

    private:
        /// Trims off unwanted data the JSON data may be surrounded with.
        /// \param data data received from the API
        static void trimJSONData(std::string &data);

    public:
        /// Creates an instance of the class.
        /// \param ip IP address of the server providing the API
        /// \param port Port the API service is running on
        RestAPIClient(std::string ip, uint16_t port);

        /// Executes the GET requests
        /// \return If everything went well, data fetched from the server will be returned.
        ///         Otherwise, an empty string "" will be returned.
        std::string fetchData();

        /// Initializes a new GET requests. Additional headers may be added afterwards.
        /// \param path path to the API e.g. /iss-now.json
        void createGETRequest(const std::string &path);

        /// Adds a header into the GET request
        /// \param key name of the header (e.g. Host)
        /// \param value value of the header (e.g. api.open-notify.org)
        void addHeader(const std::string &key, const std::string &value);
    };
}