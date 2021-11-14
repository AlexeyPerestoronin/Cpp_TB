// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include <common/str.hpp>

#include <openssl/hmac.h>

namespace TB_NS::OpenSslAdapter_NS {
    // brief: the wrapper around the OpenSSL-functional for computed of SHA-512 based on HMAC
    class HmacSha512 {
        public:
        HmacSha512(Str::CR i_key, Str::CR i_msg);

        Str compute();

        private:
        std::vector<unsigned char> m_digest;
    };
} // namespace TB_NS::OpenSslAdapter_NS
