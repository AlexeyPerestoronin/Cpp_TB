#pragma once

#include "../hmac_sha512.hpp"

#include <common/base_exception.hpp>

namespace TB_NS::OpenSslAdapter_NS {
    HmacSha512::HmacSha512(Str::CR i_key, Str::CR i_msg) {
        std::unique_ptr<HMAC_CTX, std::function<void(HMAC_CTX*)>> ctxUPtr(HMAC_CTX_new(), [](HMAC_CTX* i_ctxPtr) {
            if (i_ctxPtr)
                HMAC_CTX_free(i_ctxPtr);
        });

        // Set HMAC key.
        if (HMAC_Init_ex(ctxUPtr.get(), reinterpret_cast<const unsigned char*>(i_key.c_str()), static_cast<int>(i_key.size()), EVP_sha512(), nullptr) == NULL)
            throw TB_EXEPT["OpenSSL"]["HMAC"]["Init_ex"] << TB_LOCATION;

        // May be called repeatedly to insert all your data.
        if (HMAC_Update(ctxUPtr.get(), reinterpret_cast<const unsigned char*>(i_msg.c_str()), static_cast<int>(i_msg.size())) == NULL)
            throw TB_EXEPT["OpenSSL"]["HMAC"]["Update"] << TB_LOCATION;

        // Finish HMAC computation and fetch result.
        unsigned char result[129]{};
        unsigned int size{};
        if (HMAC_Final(ctxUPtr.get(), result, &size) == NULL)
            throw TB_EXEPT["OpenSSL"]["HMAC"]["Final"] << TB_LOCATION;

        for (unsigned int i = 0; i < size; i++) {
            m_digest.push_back(int(result[i]));
        }
    }

    Str HmacSha512::compute() {
        static const char hex_chars[]{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
        Str r_digestStr;
        for (auto i : m_digest) {
            const char byte = i;
            r_digestStr.push_back(hex_chars[(byte & 0xF0) >> 4]);
            r_digestStr.push_back(hex_chars[(byte & 0x0F) >> 0]);
        }
        return r_digestStr;
    }
} // namespace TB_NS::OpenSslAdapter_NS
