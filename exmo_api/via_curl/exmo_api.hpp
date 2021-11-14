#pragma once

#include <common/stdafx.hpp>
#include <common/json.hpp>

#include <curl_adaptor/http.hpp>


namespace TB_NS::Exmo_NS {
    class API {
        std::string m_public_key;
        std::string m_secret_key;

        Str m_url{ "api.exmo.com/v1/"s };
        ULONG m_nonce{ static_cast<ULONG>(std::time(nullptr)) };
        CurlAdapter_NS::Http_NS::Connection m_connection{ CurlAdapter_NS::Http_NS::Connection() };

        public:
        API(std::string i_public_key, std::string i_secret_key);

        Json call(std::string_view i_method, std::string_view i_params);

        static std::string build(const std::vector<std::string>& i_params);
    };
} // namespace TB_NS::Exmo_NS
