#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "date.h"

namespace psi::utils {

    std::string simplifyDatetimeFormat(const std::string &datetime);
    std::vector<std::string> split(const std::string &str, char separator);
    std::string reformatDatetime(const std::string &datetime);
    date::sys_seconds getDatetime(const std::string &datetime);
}