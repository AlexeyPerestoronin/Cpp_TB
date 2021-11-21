// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include "../exmo_api.hpp"

#include <common/aliases.hpp>

#include <openssl_adaptor/hmac_sha512.hpp>

namespace TB_NS::Exmo_NS::ViaCurl_NS {
    using namespace TB_NS::Trader_NS::Interaction_NS;

    ExmoExchange::ExmoExchange(Str i_publicKey, Str i_secretKey)
        : m_publicKey(std::move(i_publicKey))
        , m_secretKey(std::move(i_secretKey)) {}

    ExchangeCode ExmoExchange::code() const noexcept {
        return ExchangeID::EXMO;
    }

    InteractI::SP ExmoExchange::createIteractions(CommandCode i_commandCode) {
        return std::shared_ptr<InteractI>(new ExmoInteraction(i_commandCode, *this, ++m_nonce));
    }

    Str::CR ExmoExchange::method(CommandCode i_commandCode) const {
        if (i_commandCode == CommandID::GET_ORDER_BOOK) {
            static Str getOrderBook = m_url + "order_book";
            return getOrderBook;
        } else
            throw TB_EXEPT["EXMO"]["API"]["REST"]["unregistered"]("{id:}", ToStr(i_commandCode)) << TB_LOCATION;
    }

    Json ExmoExchange::request(CommandCode i_commandCode, Str::CR i_params) {
        StrToStr headers{
            { "Content-type", "application/x-www-form-urlencoded"s },
            { "Key", m_publicKey },
            { "Sign", std::move(OpenSslAdapter_NS::HmacSha512(m_secretKey, i_params).compute()) },
        };

        CurlAdapter_NS::Http_NS::PostRequest postRequest{};
        postRequest.prepare(headers);
        CurlAdapter_NS::Http_NS::Connection connection{ CurlAdapter_NS::Http_NS::Connection() };
        connection.makeRequest(method(i_commandCode), postRequest, i_params);
        return connection.getResponse();
    }

    Json ExmoExchange::call(Str::CR i_method, Str::CR i_params) {
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
        CurlAdapter_NS::Http_NS::Connection connection{ CurlAdapter_NS::Http_NS::Connection() };
        connection.makeRequest(url, postRequest, params);
        return connection.getResponse();
    }

    Str ExmoExchange::build_dd(LStrs::CR i_params) {
        Str r_params{};
        for (const auto& param : i_params)
            r_params += "&" + param;
        return r_params;
    }
} // namespace TB_NS::Exmo_NS::ViaCurl_NS
