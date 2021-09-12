#include "../_hmac_sha512.hpp"
#include "../_connection.hpp"
#include "../exmo_api.hpp"

#include <string>
#include <ctime>
#include <map>

exmo_api::exmo_api(const std::string& key, const std::string& secret) {
    key_ = key;
    secret_ = secret;
    url_ = "api.exmo.com/v1/";
    connection_ = http::connection();
    nonce_ = ::time(nullptr);
}

json_data exmo_api::call(const std::string& method, const std::string& p) {
    std::string params = "nonce=";
    nonce_++;
    params.append(std::to_string(nonce_));

    if (p.size() != 0) {
        params.append("&");
    }
    params.append(p);

    std::map<std::string, std::string> headers;
    headers["Content-type"] = "application/x-www-form-urlencoded";
    headers["Key"] = key_;
    headers["Sign"] = this->signature(params);

    connection_.request(url_ + method, http::post(), params, headers);
    return connection_.get_response();
}

std::string exmo_api::build(std::vector<std::string> params_) {
    std::string params = "";
    for (auto i : params_) {
        params += "&" + i;
    }
    return params;
}

std::string exmo_api::signature(const std::string& params) {
    HMAC_SHA512 hmac_sha512(secret_, params);
    return hmac_sha512.hex_digest();
}
