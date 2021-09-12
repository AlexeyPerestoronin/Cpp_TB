#pragma once

#include "../_hmac_sha512.hpp"

using namespace TB_NS;

HMAC_SHA512::HMAC_SHA512(const std::string& key, const std::string& msg) {
    HMAC_CTX* ctx = HMAC_CTX_new();

    // Set HMAC key.
    HMAC_Init_ex(ctx, reinterpret_cast<const unsigned char*>(key.c_str()), key.size(), EVP_sha512(), NULL);

    // May be called repeatedly to insert all your data.
    HMAC_Update(ctx, reinterpret_cast<const unsigned char*>(msg.c_str()), msg.size());

    // Finish HMAC computation and fetch result.
    unsigned char* result = new unsigned char[129];
    unsigned int result_len = 129;
    HMAC_Final(ctx, result, &result_len);
    for (int i = 0; i < result_len; i++) {
        m_digest.push_back(int(result[i]));
    }
    HMAC_CTX_free(ctx);
}

std::string HMAC_SHA512::hex_digest() {
    std::string str;
    const char hex_chars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    for (auto i : m_digest) {
        const char byte = i;
        str.push_back(hex_chars[(byte & 0xF0) >> 4]);
        str.push_back(hex_chars[(byte & 0x0F) >> 0]);
    }
    return str;
}
