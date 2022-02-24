#pragma once

#include "json.hpp"

namespace psi::iss {

    /// Fetches down information using an API. If data is fetched down successfully,
    /// an instance of nlohmann::json will be returned. Otherwise, an error message
    /// will be printed out and the program will terminate.
    /// \param ip ip address of the server providing the API
    /// \param host hostname of the server
    /// \param port port on which the service is running
    /// \param path API path containing all parameters
    /// \return JSON if all went well. Termination of the program if no data has been fetched.
    nlohmann::json fetchData(const std::string &ip, const std::string &host, uint16_t port, const std::string &path);

    /// Prints out information about the ISS.
    void printInfo();
}