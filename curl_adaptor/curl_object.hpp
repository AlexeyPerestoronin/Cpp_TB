// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include <curl/curl.h>

namespace TB_NS::CurlAdapter_NS {
    // brief: presents the entry point for the CURL library
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
} // namespace TB_NS::CurlAdapter_NS
