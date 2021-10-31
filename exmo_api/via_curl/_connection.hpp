#pragma once

#include <common/stdafx.hpp>

#include <exmo_api/via_curl/_curl_object.hpp>

namespace TB_NS {
    namespace HTTP_NS {
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
    } // namespace HTTP_NS
} // namespace TB_NS
