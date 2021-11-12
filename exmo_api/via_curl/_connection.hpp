#pragma once

#include <common/stdafx.hpp>
#include <common/aliases.hpp>

#include "_curl_object.hpp"

namespace TB_NS::HTTP_NS {
    using JsonData = std::string;
    using StrToStr = std::map<std::string, std::string>;

    class Request {
        public:
        virtual curl_slist* prepare(const StrToStr&) const = 0;
    };

    class Post : public Request {
        public:
        Post() = default;
        virtual ~Post() = default;

        curl_slist* prepare(const StrToStr& headers) const override;
    };

    class Connection {
        public:
        void request(std::string_view i_url, const Request& i_request, std::string_view i_params = "", const StrToStr& i_headers = StrToStr());

        JsonData get_response();

        private:
        static std::size_t write_received_data_to_string(char* ptr, std::size_t size, std::size_t nmemb, void* buffer);

        private:
        Str m_requestData;
        Str m_requestHeader;
    };
} // namespace TB_NS::HTTP_NS
