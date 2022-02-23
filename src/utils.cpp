#include <sstream>

#include "utils.h"

namespace psi::utils {

    std::vector<std::string> split(const std::string &str, char separator) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string x;
        while (getline(ss, x, separator)) {
            if (!x.empty()) {
                tokens.emplace_back(x);
            }
        }
        return tokens;
    }

    std::string simplifyDatetimeFormat(const std::string &datetime) {
        auto parts = split(datetime, 'T');
        std::string date = parts[0];
        std::string time = split(parts[1], '+')[0];
        return date + " " + time;
    }

    std::string reformatDatetime(const std::string &datetime) {
        std::string reformattedDatetime;
        auto parts = split(datetime, ' ');
        auto dateParts = split(parts[0], '-');
        auto timeParts = split(parts[1], ':');

        for (const auto &part : dateParts) {
            reformattedDatetime += part;
        }
        for (const auto &part : timeParts) {
            reformattedDatetime += part;
        }
        return reformattedDatetime;
    }

    date::sys_seconds getDatetime(const std::string &datetime) {
        static std::string format = " %Y%m%d%H%M%S";
        std::istringstream is {datetime};
        date::sys_seconds timestamp;
        is >> date::parse(format, timestamp);
        return timestamp;
    }
}