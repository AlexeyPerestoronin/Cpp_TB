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
        // brief: common interface of the HTTP-request for the CURL library
        class RequestI {
            TB_PUBLIC_PRS(RequestI);

            protected:
            Str m_value{};
            curl_slist* m_list{};

            void addValue(Str::CR i_value) noexcept;

            public:
            RequestI() noexcept = default;
            virtual ~RequestI();

            TB_NODISCARD curl_slist* get() noexcept;
            void reset() noexcept;
            Str::CR value() noexcept;
            virtual void prepare(StrToStr::CR) = 0;
        };

        // brief: HTTP-POST-request adapter
        class PostRequest : public RequestI {
            TB_PUBLIC_PRS(PostRequest);

            public:
            void prepare(StrToStr::CR i_headers) override;
        };

        // brief: the adapter of the CURL library HTTP-connection
        class Connection {
            TB_PUBLIC_PRS(Connection);
            
        private:
            Str m_requestData;
            Str m_requestHeader;

            public:
            void makeRequest(Str::CR i_url, RequestI& i_request, Str::CR i_params = Str{});
            Str getResponse();
        };
    } // namespace Http_NS
} // namespace TB_NS::CurlAdapter_NS::Http_NS
