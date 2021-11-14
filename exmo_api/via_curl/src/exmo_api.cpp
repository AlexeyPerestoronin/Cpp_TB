#include "../exmo_api.hpp"

#include <common/aliases.hpp>
#include <common/base_exception.hpp>

namespace TB_NS::Exmo_NS {
    API::API(std::string i_public_key, std::string i_secret_key)
        : m_public_key(std::move(i_public_key))
        , m_secret_key(std::move(i_secret_key)) {}

    Json API::call(std::string_view i_method, std::string_view i_params) {
        std::string params = "nonce=";
        m_nonce++;
        params.append(std::to_string(m_nonce));

        if (i_params.size() != 0) {
            params.append("&");
        }
        params.append(i_params);

        StrToStr headers;
        headers["Content-type"] = "application/x-www-form-urlencoded"s;
        headers["Key"] = m_public_key;
        headers["Sign"] = OpenSSL_NS::HmacSha512(m_secret_key, params).compute();

        // TODO: there need eliminate coping by using a std::map<COMMAND_ID command_id, std::string target_url>;
        auto url = std::string(m_url).append(i_method);
        CurlAdapter_NS::Http_NS::PostRequest postRequest{};
        postRequest.prepare(headers);
        m_connection.makeRequest(url, postRequest, params);
        return m_connection.getResponse();
    }

    std::string API::build(const std::vector<std::string>& i_params) {
        std::string r_params{};
        for (const auto& param : i_params)
            r_params += "&" + param;
        return r_params;
    }
} // namespace TB_NS::Exmo_NS
