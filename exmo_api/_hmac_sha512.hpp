#pragma once

#include <openssl/hmac.h>
#include <string>
#include <vector>

class HMAC_SHA512 {
public:
    HMAC_SHA512(const std::string& key, const std::string& msg);

	std::string hex_digest();

private:
	std::vector<unsigned char> m_digest;
};
