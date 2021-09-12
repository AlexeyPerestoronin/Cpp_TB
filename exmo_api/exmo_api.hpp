#pragma once

#include "./_hmac_sha512.hpp"
#include "./_connection.hpp"

class exmo_api {
public:
	exmo_api(const std::string& key, const std::string& secret);

	json_data call(const std::string& method, const std::string& p);

	std::string build(std::vector<std::string> params_);

private:
	std::string signature(const std::string& params);

private:
	unsigned long nonce_;
	std::string key_;
	std::string secret_;
	std::string url_;
	http::connection connection_;
};
