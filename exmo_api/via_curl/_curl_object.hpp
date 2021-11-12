// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include <curl/curl.h>

namespace TB_NS {
    namespace Curl_NS {
        class EntryPoint {
            static CURL* m_curlPtr;

            public:
            EntryPoint() = delete;
            EntryPoint(EntryPoint&&) = delete;
            EntryPoint& operator=(EntryPoint&&) = delete;
            EntryPoint(const EntryPoint&) = delete;
            EntryPoint& operator=(const EntryPoint&) = delete;

            ~EntryPoint();

            static CURL* GetIns();
        };
    } // namespace CURL_NS
} // namespace TB_NS
