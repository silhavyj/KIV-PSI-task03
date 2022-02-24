#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "date.h"

/// General util functions used throughout the application.
namespace psi::utils {

    /// Converts a datetime in the following format "2022-02-24T09:37:02+00:00"
    /// into a simplified format - "2022-02-24 09:37:02".
    /// \param datetime the datetime be converted
    /// \return the same datetime in a simplified format
    std::string simplifyDatetimeFormat(const std::string &datetime);

    /// Splits text by a specific character passed as an arguments.
    /// \param str text (string) to be split up
    /// \param separator deliminator by which the text will be split up
    /// \return individual tokens (result of splitting up)
    std::vector<std::string> split(const std::string &str, char separator);

    /// Converts a datetime in the following format "2022-02-24 09:37:02"
    /// into "20220224093702". This format is further used as an input when
    /// converting a string into date::sys_seconds.
    /// \param datetime datetime to be converted e.g. "2022-02-24 09:37:02"
    /// \return datetime in a converted format e.g. "20220224093702"
    std::string reformatDatetime(const std::string &datetime);

    /// Returns an instance of date::sys_seconds after passing in
    /// a datetime in the following format "20220224093702".
    /// \param datetime datetime in a string format
    /// \return instance of date::sys_seconds (used for arithmetics operations)
    date::sys_seconds getDatetime(const std::string &datetime);
}