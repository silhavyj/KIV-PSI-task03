#pragma once

#include "json.hpp"

namespace psi::iss {

    nlohmann::json fetchData(const std::string &ip, const std::string &host, uint16_t port, const std::string &path);
    void printInfo();
}