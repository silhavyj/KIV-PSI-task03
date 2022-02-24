#include <iostream>

#include "utils.h"
#include "date.h"
#include "json.hpp"
#include "iss.h"
#include "config.h"
#include "restapiclient.h"

namespace psi::iss {

    nlohmann::json fetchData(const std::string &ip, const std::string &host, uint16_t port, const std::string &path) {
        // Initialize a new GET request call.
        RestAPIClient apiClient(ip, port);
        apiClient.createGETRequest(path);

        // Add necessary headers, so the request goes through.
        apiClient.addHeader("Host", host);
        apiClient.addHeader("Connection", "close");
        apiClient.addHeader("Upgrade-Insecure-Requests", "1");

        // Executes the call.
        std::string rawData = apiClient.fetchData();

        // Check if some data has been returned. If not, terminate the program.
        if (rawData.empty()) {
            std::cout << "ERR: no data has been received from the server\n";
            exit(EXIT_FAILURE);
        }

        // Convert the string into nlohmann::json and return it.
        auto data = nlohmann::json::parse(rawData);
        return data;
    }

    void printInfo() {
        // Print out the header (info)
        std::cout << "Information about the International Space Station (ISS)\n";
        std::cout << "-------------------------------------------------------\n";

        bool issDaylight;

        // Fetch the current information about the ISS.
        auto issJSON = fetchData(config::iss::IP,
                                 config::iss::HOST,
                                 config::iss::PORT,
                                 config::iss::PATH);

        // Store the current position long with the timestamp into local variables.
        std::string latitude = issJSON["iss_position"]["latitude"];
        std::string longitude = issJSON["iss_position"]["longitude"];
        long issTimestamp = issJSON["timestamp"];

        // Print out the current latitude and longitude.
        std::cout << "latitude: " << latitude << '\n';
        std::cout << "longitude: " << longitude << '\n';

        // Pass the latitude and longitude into the next API call.
        std::string sunriseSunsetPath = "/json?lat=" + latitude + "&lng=" + longitude + "&formatted=0";

        // Fetch down information about sunrise/sunset based on the current position of the ISS.
        auto sunriseSunsetJSON = fetchData(config::sunrise_sunset::IP,
                                           config::sunrise_sunset::HOST,
                                           config::sunrise_sunset::PORT,
                                           sunriseSunsetPath);

        // Store the time of sunrise/sunset into local variables.
        std::string sunriseRaw = sunriseSunsetJSON["results"]["sunrise"];
        std::string sunsetRaw = sunriseSunsetJSON["results"]["sunset"];

        // Simplify the format of the time of sunrise/sunset.
        std::string simplifiedSunriseTimestamp = utils::simplifyDatetimeFormat(sunriseRaw);
        std::string simplifiedSunsetTimestamp = utils::simplifyDatetimeFormat(sunsetRaw);

        // Pass the unit timestamp into the next API call, so we have all three times in the same format.
        std::string timestampPath = "/UnixTime/fromunixtimestamp?unixtimestamp=" + std::to_string(issTimestamp);
        auto timestampJSON = fetchData(config::timestamp::IP,
                                       config::timestamp::HOST,
                                       config::timestamp::PORT,
                                       timestampPath);

        // Store the converted datetime into a local variable and
        // print it out onto the screen.
        std::string issLastUpdate = timestampJSON["Datetime"];
        std::cout << "last update: " << issLastUpdate << " (UTC)\n";

        // Convert all three datetimes into date::sys_seconds, so
        // we can perform arithmetics operations with them.
        date::sys_seconds sunriseTime = utils::getDatetime(utils::reformatDatetime(simplifiedSunriseTimestamp));
        date::sys_seconds sunsetTime = utils::getDatetime(utils::reformatDatetime(simplifiedSunsetTimestamp));
        date::sys_seconds issTime = utils::getDatetime(utils::reformatDatetime(issLastUpdate));

        {
            using namespace date;

            // Check if the ISS is currently in darkness or not.
            issDaylight = issTime > sunriseTime && issTime < sunsetTime;
            if (issDaylight) {
                std::cout << "ISS is currently in light\n";
            } else {
                std::cout << "ISS is currently in darkness\n";
            }
        }

        // Calculate the absolute difference from sunrise/sunset. The difference is in hours.
        long secondsBeforeSunrise = abs(sunriseTime - issTime).count();
        long secondsAfterSunset = abs(sunsetTime - issTime).count();
        auto hoursBeforeSunrise = static_cast<float>(secondsBeforeSunrise / 3600.0);
        auto hoursAfterSunset = static_cast<float>(secondsAfterSunset / 3600.0);

        std::cout << "abs diff from sunrise (hours): " <<  hoursBeforeSunrise << '\n';
        std::cout << "abs diff from sunset (hours): " <<  hoursAfterSunset << '\n';
        std::cout << "\n";

        // Check if it;s the perfect time to observe the ISS.
        if ((issTime >= sunsetTime && hoursAfterSunset >= 1 && hoursAfterSunset <= 2) || (issTime <= sunriseTime && hoursBeforeSunrise >= 1 && hoursBeforeSunrise <= 2)) {
            std::cout << "It's the perfect time to observe the ISS fly by!\n";
        } else {
            std::cout << "It's NOT the perfect time to observe the ISS fly by :(\n";
            if (issDaylight) {
                std::cout << "You have to wait until it passes sunset (1-2 hours after the sun goes down)\n";
            } else {
                std::cout << "You have to wait until it gets closer to sunrise (1-2 hours before the sun comes up)\n";
            }
        }
        std::cout << "\nYou can verify the answers over at https://spotthestation.nasa.gov/tracking_map.cfm\n";
    }
}