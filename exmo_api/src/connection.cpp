#pragma once

#include "../_curl_object.hpp"
#include "../_connection.hpp"

namespace http {
    connection::connection(const std::string url) {}

    void connection::request(const std::string url, const http::request& r, const std::string& params, const headers_t& headers) {
        recv_data_.clear();
        recv_header_.clear();
        //curl_easy_setopt(curl_object::get_instance(), CURLOPT_SSL_VERIFYHOST, 0);
        //curl_easy_setopt(curl_object::get_instance(), CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl_object::get_instance(), CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_object::get_instance(), CURLOPT_WRITEDATA, &recv_data_);
        curl_easy_setopt(curl_object::get_instance(), CURLOPT_HEADERDATA, &recv_header_);
        curl_easy_setopt(curl_object::get_instance(), CURLOPT_WRITEFUNCTION, write_received_data_to_string);

        curl_easy_setopt(curl_object::get_instance(), CURLOPT_POSTFIELDS, params.c_str());
        curl_easy_setopt(curl_object::get_instance(), CURLOPT_POSTFIELDSIZE, params.size());

        curl_slist* list = r.prepare(headers);
        curl_easy_setopt(curl_object::get_instance(), CURLOPT_HTTPHEADER, list);

        CURLcode rescode = curl_easy_perform(curl_object::get_instance());
        if (rescode != CURLE_OK) {
            std::string msg{ "!> curl_easy_perform failed with error: " };
            msg += curl_easy_strerror(rescode);
            throw std::runtime_error(msg);
        }
    }

    json_data connection::get_response() {
        return recv_data_;
    }

    std::size_t connection::write_received_data_to_string(char* ptr, std::size_t size, std::size_t nmemb, void* buffer) {
        if (buffer) {
            std::string* tstr = reinterpret_cast<std::string*>(buffer);
            tstr->append(ptr, size * nmemb);
        }
        return size * nmemb;
    }
} // namespace http
