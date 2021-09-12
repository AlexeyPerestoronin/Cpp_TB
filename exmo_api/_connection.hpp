#pragma once

#include "./_curl_object.hpp"

#include <stdexcept>
#include <iostream>
#include <string>
#include <map>

using json_data = std::string;
using headers_t = std::map<std::string, std::string>;

namespace http {

    class request {
        public:
        request() {}
        virtual ~request() {}

        virtual curl_slist* prepare(const headers_t&) const = 0;
    };

    class post : public request {
        public:
        post() {}
        virtual ~post() {}

        virtual curl_slist* prepare(const headers_t& headers) const {
            curl_slist* list = nullptr;
            for (auto h : headers) {
                std::string s{ std::string(h.first + ": " + h.second) };
                list = curl_slist_append(list, s.c_str());
            }
            return list;
        }
    };

    class connection {
        public:
        connection(const std::string url = "");
        ~connection() = default;

        void request(const std::string url, const request& r, const std::string& params = "", const headers_t& headers = headers_t());

        json_data get_response();

        private:
        static std::size_t write_received_data_to_string(char* ptr, std::size_t size, std::size_t nmemb, void* buffer);

        private:
        std::string recv_data_;
        std::string recv_header_;
    };
} // namespace http
