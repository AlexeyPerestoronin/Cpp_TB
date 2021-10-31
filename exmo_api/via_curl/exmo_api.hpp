#pragma once

#include <common/stdafx.hpp>

#include <exmo_api/via_curl/_hmac_sha512.hpp>
#include <exmo_api/via_curl/_connection.hpp>

namespace TB_NS {
    namespace Exmo_NS {
        class API {
            ULONG m_nonce;
            std::string m_url;
            std::string m_public_key;
            std::string m_secret_key;
            HTTP_NS::Connection m_connection;

            public:
            API(std::string i_public_key, std::string i_secret_key);

            HTTP_NS::JsonData call(std::string_view i_method, std::string_view i_params);

            static std::string build(const std::vector<std::string>& i_params);

            private:
            std::string signature(const std::string& params);
        };
    } // namespace Exmo_NS
} // namespace TB_NS
