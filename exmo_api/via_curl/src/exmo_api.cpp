#include "../exmo_api.hpp"

#include <common/aliases.hpp>

#include <openssl_adaptor/hmac_sha512.hpp>

namespace TB_NS::Exmo_NS {
    using namespace TB_NS::Trader_NS::Interaction_NS;

    API::API(Str i_publicKey, Str i_secretKey)
        : m_publicKey(std::move(i_publicKey))
        , m_secretKey(std::move(i_secretKey)) {}

    ExchangeCode API::code() const noexcept {
        return ExchangeID::EXMO;
    }

    void API::check(CommandCode i_commandCode, CurrencyPair& io_pair) const {
        // TODO: for to be possible to check the currency pair it is need to know what pairs are presented in the Exchange platform
        return;
    }

    void API::check(CommandCode i_commandCode, Limit& io_limit) const {
#define REST_API_EXCEPT TB_EXEPT["EXMO"]["API"]["REST"]
        if (i_commandCode == CommandID::GET_ORDER_BOOK) {
            static Int minAllowed{ 100 };
            if (!io_limit.has_value())
                io_limit.emplace(minAllowed);
            else if (io_limit < minAllowed)
                throw REST_API_EXCEPT["order_book"]["limits"]("{min:}", ToStr(minAllowed))("{requested:}", ToStr(io_limit.value())) << TB_LOCATION;
        } else
            throw REST_API_EXCEPT["unregistered"]("{id:}", ToStr(i_commandCode)) << TB_LOCATION;
#undef REST_API_EXCEPT
    }

    Json API::interact(CommandCode i_commandCode, RequestedParameters::CR i_params) {
        return Json();
    }

    Json API::call(Str::CR i_method, Str::CR i_params) {
        Str params = "nonce=";
        m_nonce++;
        params.append(std::to_string(m_nonce));

        if (i_params.size() != 0) {
            params.append("&");
        }
        params.append(i_params);

        StrToStr headers;
        headers["Content-type"] = "application/x-www-form-urlencoded"s;
        headers["Key"] = m_publicKey;
        headers["Sign"] = OpenSslAdapter_NS::HmacSha512(m_secretKey, params).compute();

        // TODO: there need eliminate coping by using a std::map<COMMAND_ID i_commandCode, Str target_url>;
        auto url = Str(m_url).append(i_method);
        CurlAdapter_NS::Http_NS::PostRequest postRequest{};
        postRequest.prepare(headers);
        m_connection.makeRequest(url, postRequest, params);
        return m_connection.getResponse();
    }

    Str API::build(LStrs::CR i_params) {
        Str r_params{};
        for (const auto& param : i_params)
            r_params += "&" + param;
        return r_params;
    }
} // namespace TB_NS::Exmo_NS
