#pragma once

#include <common/stdafx.hpp>

#include <openssl/hmac.h>

namespace TB_NS {
    namespace OpenSSL_NS {
        class HMAC_SHA512 {
            public:
            HMAC_SHA512(Str::CR i_key, Str::CR i_msg);

            std::string hex_digest();

            private:
            std::vector<unsigned char> m_digest;
        };
    } // namespace OpenSSL_NS
} // namespace TB_NS
