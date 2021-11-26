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
            throw TB_EXEPT["EXMO"]["API"]["REST"]["Unregistered Request"]("{id:}", ToStr(i_commandCode)) << TB_LOCATION;
    }

    Json ExmoExchange::request(CommandCode i_commandCode, Str::CR i_params) {
#define MANGLED_RESPONSE TB_EXEPT["EXMO"]["API"]["REST"]["Mangled Response"]("{id:}", ToStr(i_commandCode))("{params:}", i_params)
        try {
            StrToStr headers{
                { "Content-type", "application/x-www-form-urlencoded"s },
                { "Key", m_publicKey },
                { "Sign", std::move(OpenSslAdapter_NS::HmacSha512(m_secretKey, i_params).compute()) },
            };

            CurlAdapter_NS::Http_NS::PostRequest postRequest{};
            postRequest.prepare(headers);
            CurlAdapter_NS::Http_NS::Connection connection{ CurlAdapter_NS::Http_NS::Connection() };
            connection.makeRequest(method(i_commandCode), postRequest, i_params);
            Str response = connection.getResponse();
            try {
                return Json::parse(response);
            } catch (std::exception& error) {
                throw MANGLED_RESPONSE("{json:}", response)("{error:}", error.what()) << TB_LOCATION;
            }
        } catch (Error_NS::Exception& error) {
            throw MANGLED_RESPONSE << TB_SUBERROR(error) << TB_LOCATION;
        }
#undef MANGLED_RESPONSE
    }
} // namespace TB_NS::Exmo_NS::ViaCurl_NS
