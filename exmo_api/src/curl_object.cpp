#pragma once

#include "../_curl_object.hpp"

using namespace TB_NS;

CURL* curl_object::ch = nullptr;

curl_object::~curl_object() {
    curl_easy_cleanup(ch);
    ch = nullptr;
}

CURL* curl_object::get_instance() {
    if (ch == nullptr) {
        ch = curl_easy_init();
    }
    return ch;
}
