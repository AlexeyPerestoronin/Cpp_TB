#pragma once

#include <curl/curl.h>

namespace TB_NS {
    class curl_object {
        static CURL* ch;

        public:
        curl_object() = delete;

        ~curl_object();

        static CURL* get_instance();
    };
} // namespace TB_NS
