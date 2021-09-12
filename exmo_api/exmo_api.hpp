#pragma once

#include "./_hmac_sha512.hpp"
#include "./_connection.hpp"

namespace TB_NS {
    class ExmoApi {
        ULONG m_nonce;
        std::string m_url; 
        std::string m_public_key;
        std::string m_secret_key;
        http::Connection m_connection;
    
        public:
        ExmoApi(std::string i_public_key, std::string i_secret_key);

        http::JsonData call(std::string_view i_method, std::string_view i_params);

        std::string build(std::vector<std::string> i_params);

        private:
        std::string signature(const std::string& params);
    };
} // namespace TB_NS
