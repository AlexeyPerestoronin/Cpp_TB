// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include <common/stdafx.hpp>
#include <common/aliases.hpp>

#include "curl_object.hpp"

namespace TB_NS::CurlAdapter_NS {
    namespace Http_NS {
        using JsonData = std::string;
        using StrToStr = std::map<std::string, std::string>;

        class Request {
            TB_PUBLIC_PRS(Request);

            protected:
            curl_slist* m_list{};

            public:
            virtual ~Request() = default;
            virtual curl_slist* prepare(const StrToStr&) = 0;
        };

        class PostRequest : public Request {
            TB_PUBLIC_PRS(PostRequest);

            public:
            PostRequest();
            ~PostRequest();

            curl_slist* prepare(const StrToStr& headers) override;
        };

        class Connection {
            public:
            void request(std::string_view i_url, Request& i_request, std::string_view i_params = "", const StrToStr& i_headers = StrToStr());

            JsonData get_response();

            private:
            static std::size_t write_received_data_to_string(char* ptr, std::size_t size, std::size_t nmemb, void* buffer);

            private:
            Str m_requestData;
            Str m_requestHeader;
        };
    } // namespace Http_NS
} // namespace TB_NS::CurlAdapter_NS::Http_NS
