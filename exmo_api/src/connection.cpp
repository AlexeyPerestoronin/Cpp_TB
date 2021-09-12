#pragma once

#include <stdexcept>

#include "../_curl_object.hpp"
#include "../_connection.hpp"

using namespace TB_NS;
using namespace TB_NS::http;

curl_slist* Post::prepare(const Headers& headers) const {
    curl_slist* list = nullptr;
    for (auto h : headers) {
        std::string s{ std::string(h.first + ": " + h.second) };
        list = curl_slist_append(list, s.c_str());
    }
    return list;
}

void Connection::request(std::string_view i_url, const http::Request& i_request, std::string_view i_params, const Headers& i_headers) {
    recv_data_.clear();
    recv_header_.clear();
    //curl_easy_setopt(curl_object::get_instance(), CURLOPT_SSL_VERIFYHOST, 0);
    //curl_easy_setopt(curl_object::get_instance(), CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl_object::get_instance(), CURLOPT_URL, i_url.data());
    curl_easy_setopt(curl_object::get_instance(), CURLOPT_WRITEDATA, &recv_data_);
    curl_easy_setopt(curl_object::get_instance(), CURLOPT_HEADERDATA, &recv_header_);
    curl_easy_setopt(curl_object::get_instance(), CURLOPT_WRITEFUNCTION, write_received_data_to_string);

    curl_easy_setopt(curl_object::get_instance(), CURLOPT_POSTFIELDS, i_params.data());
    curl_easy_setopt(curl_object::get_instance(), CURLOPT_POSTFIELDSIZE, i_params.size());

    curl_slist* list = i_request.prepare(i_headers);
    curl_easy_setopt(curl_object::get_instance(), CURLOPT_HTTPHEADER, list);

    CURLcode rescode = curl_easy_perform(curl_object::get_instance());
    if (rescode != CURLE_OK) {
        std::string msg{ "!> curl_easy_perform failed with error: " };
        msg += curl_easy_strerror(rescode);
        throw std::runtime_error(msg);
    }
}

JsonData Connection::get_response() {
    return recv_data_;
}

std::size_t Connection::write_received_data_to_string(char* ptr, std::size_t size, std::size_t nmemb, void* buffer) {
    if (buffer) {
        std::string* tstr = reinterpret_cast<std::string*>(buffer);
        tstr->append(ptr, size * nmemb);
    }
    return size * nmemb;
}
