#pragma once

#include <cstdint>

namespace psi::config {

    namespace iss {
        static constexpr const char *IP = "138.68.39.196";
        static constexpr const char *HOST = "api.open-notify.org";
        static constexpr const uint16_t PORT = 80;
        static constexpr const char *PATH = "/iss-now.json";
    }

    namespace sunrise_sunset {
        static constexpr const char *IP = "45.33.59.78";
        static constexpr const char *HOST = "api.sunrise-sunset.org";
        static constexpr const uint16_t PORT = 80;
    }

    namespace timestamp {
        static constexpr const char *IP = "13.79.230.33";
        static constexpr const char *HOST = "showcase.api.linx.twenty57.net";
        static constexpr const uint16_t PORT = 80;
    }
}