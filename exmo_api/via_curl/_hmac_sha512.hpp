#pragma once

#include <common/str.hpp>

#include <openssl/hmac.h>

namespace TB_NS {
    namespace OpenSSL_NS {
        class HmacSha512 {
            public:
            HmacSha512(Str::CR i_key, Str::CR i_msg);

            Str compute();

            private:
            std::vector<unsigned char> m_digest;
        };
    } // namespace OpenSSL_NS
} // namespace TB_NS
