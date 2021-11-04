#pragma once

#include <exmo_api/via_curl/_hmac_sha512.hpp>

#include <common/rai.hpp>

namespace TB_NS::OpenSSL_NS {
    HMAC_SHA512::HMAC_SHA512(Str::CR i_key, Str::CR i_msg) {
        std::unique_ptr<HMAC_CTX, std::function<void(HMAC_CTX*)>> ctxUPtr(HMAC_CTX_new(), [](HMAC_CTX* i_ctxPtr) {
            if (i_ctxPtr)
                HMAC_CTX_free(i_ctxPtr);
        });

        // Set HMAC key.
        HMAC_Init_ex(ctxUPtr.get(), reinterpret_cast<const unsigned char*>(i_key.c_str()), static_cast<int>(i_key.size()), EVP_sha512(), nullptr);

        // May be called repeatedly to insert all your data.
        HMAC_Update(ctxUPtr.get(), reinterpret_cast<const unsigned char*>(i_msg.c_str()), static_cast<int>(i_msg.size()));

        // Finish HMAC computation and fetch result.
        std::array<unsigned char, 129> result{};
        unsigned int size{};
        HMAC_Final(ctxUPtr.get(), result.data(), &size);
        for (unsigned int i = 0; i < size; i++) {
            m_digest.push_back(int(result[i]));
        }
    }

    std::string HMAC_SHA512::hex_digest() {
        static const char hex_chars[]{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
        std::string r_digestStr;
        for (auto i : m_digest) {
            const char byte = i;
            r_digestStr.push_back(hex_chars[(byte & 0xF0) >> 4]);
            r_digestStr.push_back(hex_chars[(byte & 0x0F) >> 0]);
        }
        return r_digestStr;
    }
} // namespace TB_NS::OpenSSL_NS
