#include "../exmo_api.hpp"

#include <common/aliases.hpp>

#include <openssl_adaptor/hmac_sha512.hpp>

namespace TB_NS::Exmo_NS {
#define EXMO_EXCEPT TB_EXEPT["EXMO"]
    using namespace TB_NS::Trader_NS::Interaction_NS;

    API::API(Str i_publicKey, Str i_secretKey)
        : m_publicKey(std::move(i_publicKey))
        , m_secretKey(std::move(i_secretKey)) {}

    ExchangeCode API::code() const noexcept {
        return ExchangeID::EXMO;
    }

#pragma region build(s)
    Str API::method(CommandCode i_commandCode) const {
        if (i_commandCode == CommandID::GET_ORDER_BOOK) {
            return "order_book";
        } else
            throw EXMO_EXCEPT["API"]["REST"]["unregistered"]("{id:}", ToStr(i_commandCode)) << TB_LOCATION;
    }

    Str API::build(TradePair::CR i_pair) const {
        auto recognize = [](const CurrencyCode& i_currency) -> Str::CR {
            static std::map<CurrencyID, Str> idToValue{
                { CurrencyID::BTC, "BTC" },
                { CurrencyID::RUB, "RUB" },
                { CurrencyID::USD, "USD" },
            };
            for (const auto& [id, value] : idToValue)
                if (i_currency == id)
                    return value;
            throw TB_EXEPT["EXMO"]["builds"]["unsupported_currency"]("{type:}", ToStr(i_currency)) << TB_LOCATION;
        };
        return Str{ "pair={1}_{2}" }.format("{1}", recognize(i_pair._1)).format("{2}", recognize(i_pair._2));
    }

    Str API::build(Limit::CR i_limit) const {
        return Str{ "limit={1}" }.format("{1}", ToStr(i_limit.value()));
    }

    Str API::build(RequestedParameters::CR i_params) const {
        Str r_params{ "nonce=" };
        r_params += ToStr(++m_nonce);
        /*if (i_params.size() != 0) {
            params.append("&");
        }*/
        for (auto& param : i_params)
            if (const auto* tradePair = std::get_if<TradePair>(&param); tradePair)
                r_params += "&" + build(*tradePair);
            else if (const auto* limit = std::get_if<Limit>(&param); limit)
                r_params += "&" + build(*limit);
            else
                throw TB_EXEPT["EXMO"]["builds"]["usupported_type"] << TB_LOCATION;
        return r_params;
    }
#pragma endregion

#pragma region check(s)
    void API::check(CommandCode i_commandCode, TradePair& io_pair) const {
        // TODO: for to be possible to check the currency pair it is need to know what pairs are presented in the Exchange platform
        return;
    }

    void API::check(CommandCode i_commandCode, Limit& io_limit) const {
        if (i_commandCode == CommandID::GET_ORDER_BOOK) {
            static Int min{ 1 }, def{ 100 }, max{ 1000 };
            if (!io_limit.has_value())
                io_limit.emplace(def);
            else if (const auto& value = io_limit.value(); value < min || value > max)
                // clang-format off
                throw EXMO_EXCEPT["API"]["REST"]["violation"]["limit"]
                ("{min:}", ToStr(min))
                ("{max:}", ToStr(max))
                ("{requested:}", ToStr(value))
                ("{id:}", ToStr(i_commandCode)) << TB_LOCATION;
            // clang-format on
        } else
            throw EXMO_EXCEPT["API"]["REST"]["unregistered"]("{id:}", ToStr(i_commandCode)) << TB_LOCATION;
    }

    void API::check(CommandCode i_commandCode, RequestedParameters& i_params) const {
        for (auto& param : i_params)
            if (auto* tradePair = std::get_if<TradePair>(&param); tradePair)
                check(i_commandCode, *tradePair);
            else if (auto* limit = std::get_if<Limit>(&param); limit)
                check(i_commandCode, *limit);
            else
                throw TB_EXEPT["EXMO"]["checks"]["unrecognized"] << TB_LOCATION;
    }
#pragma endregion

    Json API::interact(CommandCode i_commandCode, RequestedParameters i_params) {
        check(i_commandCode, i_params);
        Str params = build(i_params);
        StrToStr headers{ { "Content-type", "application/x-www-form-urlencoded" },
                          { "Key", m_publicKey },
                          { "Sign", OpenSslAdapter_NS::HmacSha512(m_secretKey, params).compute() } };

        auto url = Str(m_url) += method(i_commandCode);
        CurlAdapter_NS::Http_NS::PostRequest postRequest{};
        postRequest.prepare(headers);
        m_connection.makeRequest(url, postRequest, params);
        return m_connection.getResponse();
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

    Str API::build_dd(LStrs::CR i_params) {
        Str r_params{};
        for (const auto& param : i_params)
            r_params += "&" + param;
        return r_params;
    }
#undef EXMO_EXCEPT
} // namespace TB_NS::Exmo_NS
