#pragma once

#include "../curl_adapter.hpp"

using namespace TB_NS;

CURL* CurlEasy::ch = nullptr;

CurlEasy::~CurlEasy() {
    curl_easy_cleanup(ch);
    ch = nullptr;
}

CURL* CurlEasy::get_instance() {
    if (ch == nullptr) {
        ch = curl_easy_init();
    }
    return ch;
}

CurlEasy TB_NS::MakeCurlEasy() {
    return CurlEasy{};
}
