#include <exmo_api/via_curl/exmo_api.hpp>

namespace TB_NS::Exmo_NS {
    API::API(std::string i_public_key, std::string i_secret_key)
        : m_public_key(std::move(i_public_key))
        , m_secret_key(std::move(i_secret_key))
        , m_url("api.exmo.com/v1/")
        , m_connection(HTTP_NS::Connection())
        , m_nonce(static_cast<ULONG>(std::time(nullptr))) {}

    HTTP_NS::JsonData API::call(std::string_view i_method, std::string_view i_params) {
        std::string params = "nonce=";
        m_nonce++;
        params.append(std::to_string(m_nonce));

        if (i_params.size() != 0) {
            params.append("&");
        }
        params.append(i_params);

        std::map<std::string, std::string> headers;
        headers["Content-type"] = "application/x-www-form-urlencoded";
        headers["Key"] = m_public_key;
        headers["Sign"] = this->signature(params);

        // TODO: there need eliminate coping by using a std::map<COMMAND_ID command_id, std::string target_url>;
        auto url = std::string(m_url).append(i_method);
        m_connection.request(url, HTTP_NS::Post(), params, headers);
        return m_connection.get_response();
    }

    std::string API::build(const std::vector<std::string>& i_params) {
        std::string r_params{};
        for (const auto& param : i_params)
            r_params += "&" + param;
        return r_params;
    }

    std::string API::signature(const std::string& params) {
        OpenSSL_NS::HMAC_SHA512 hmac_sha512(m_secret_key, params);
        return hmac_sha512.hex_digest();
    }
} // namespace TB_NS::Exmo_NS
