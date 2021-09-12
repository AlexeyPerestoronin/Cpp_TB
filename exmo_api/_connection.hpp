#pragma once

#include "./_curl_object.hpp"

#include <string>
#include <map>

namespace TB_NS {
    namespace http {
        using JsonData = std::string;
        using Headers = std::map<std::string, std::string>;

        class Request {
            public:
            virtual curl_slist* prepare(const Headers&) const = 0;
        };

        class Post : public Request {
            public:
            Post() = default;
            virtual ~Post() = default;

            curl_slist* prepare(const Headers& headers) const override;
        };

        class Connection {
            public:

            void request(std::string_view i_url, const Request& i_request, std::string_view i_params = "", const Headers& i_headers = Headers());

            JsonData get_response();

            private:
            static std::size_t write_received_data_to_string(char* ptr, std::size_t size, std::size_t nmemb, void* buffer);

            private:
            std::string recv_data_;
            std::string recv_header_;
        };
    } // namespace http
} // namespace TB_NS
