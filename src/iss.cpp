#include <iostream>

#include "utils.h"
#include "date.h"
#include "json.hpp"
#include "iss.h"
#include "config.h"
#include "restapiclient.h"

namespace psi::iss {

    nlohmann::json fetchData(const std::string &ip, const std::string &host, uint16_t port, const std::string &path) {
        RestAPIClient apiClient(ip, port);
        apiClient.createGETRequest(path);

        apiClient.addHeader("Host", host);
        apiClient.addHeader("Connection", "close");
        apiClient.addHeader("Upgrade-Insecure-Requests", "1");

        std::string rawData = apiClient.fetchData();
        if (rawData.empty()) {
            std::cout << "ERR: no data has been received from the server\n";
            exit(EXIT_FAILURE);
        }
        auto data = nlohmann::json::parse(rawData);
        return data;
    }

    void printInfo() {
        std::cout << "Information about the International Space Station (ISS)\n";
        std::cout << "-------------------------------------------------------\n";

        bool daylight;
        auto issJSON = fetchData(config::iss::IP,
                                 config::iss::HOST,
                                 config::iss::PORT,
                                 config::iss::PATH);

        std::string latitude = issJSON["iss_position"]["latitude"];
        std::string longitude = issJSON["iss_position"]["longitude"];
        long issTimestamp = issJSON["timestamp"];

        std::cout << "latitude: " << latitude << '\n';
        std::cout << "longitude: " << longitude << '\n';

        std::string sunriseSunsetPath = "/json?lat=" + latitude + "&lng=" + longitude + "&formatted=0";
        auto sunriseSunsetJSON = fetchData(config::sunrise_sunset::IP,
                                           config::sunrise_sunset::HOST,
                                           config::sunrise_sunset::PORT,
                                           sunriseSunsetPath);

        std::string timestampPath = "/UnixTime/fromunixtimestamp?unixtimestamp=" + std::to_string(issTimestamp);
        auto timestampJSON = fetchData(config::timestamp::IP,
                                       config::timestamp::HOST,
                                       config::timestamp::PORT,
                                       timestampPath);

        std::string issLastUpdate = timestampJSON["Datetime"];
        std::cout << "last update: " << issLastUpdate << " (UTC)\n";

        std::string sunriseRaw = sunriseSunsetJSON["results"]["sunrise"];
        std::string sunsetRaw = sunriseSunsetJSON["results"]["sunset"];

        std::string simplifiedSunriseTimestamp = utils::simplifyDatetimeFormat(sunriseRaw);
        std::string simplifiedSunsetTimestamp = utils::simplifyDatetimeFormat(sunsetRaw);

        date::sys_seconds sunriseTime = utils::getDatetime(utils::reformatDatetime(simplifiedSunriseTimestamp));
        date::sys_seconds sunsetTime = utils::getDatetime(utils::reformatDatetime(simplifiedSunsetTimestamp));
        date::sys_seconds issTime = utils::getDatetime(utils::reformatDatetime(issLastUpdate));

        {
            using namespace date;
            daylight = issTime > sunriseTime && issTime < sunsetTime;
            if (daylight) {
                std::cout << "ISS is currently in light\n";
            } else {
                std::cout << "ISS is currently in light\n";
            }
        }

        long secondsBeforeSunrise = abs(sunriseTime - issTime).count();
        long secondsAfterSunset = abs(sunsetTime - issTime).count();

        auto hoursBeforeSunrise = static_cast<float>(secondsBeforeSunrise / 3600.0);
        auto hoursAfterSunset = static_cast<float>(secondsAfterSunset / 3600.0);

        std::cout << "abs diff from sunrise (hours): " <<  hoursBeforeSunrise << '\n';
        std::cout << "abs diff from sunset (hours): " <<  hoursAfterSunset << '\n';

        std::cout << "\n";
        if ((issTime >= sunsetTime && hoursAfterSunset >= 1 && hoursAfterSunset <= 2) || (issTime <= sunriseTime && hoursBeforeSunrise >= 1 && hoursBeforeSunrise <= 2)) {
            std::cout << "It's the perfect time to observe the ISS fly by!\n";
        } else {
            std::cout << "It's NOT the perfect time to observe the ISS fly by :(\n";
            std::cout << "You have to wait until it gets closer to ";
            if (!daylight) {
                std::cout << "sunrise\n";
            } else {
                std::cout << "sunset\n";
            }
        }
        std::cout << "You can verify the answers over at https://spotthestation.nasa.gov/tracking_map.cfm\n";
    }
}