// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include "../exmo_api.hpp"

#include <openssl_adaptor/hmac_sha512.hpp>

namespace TB_NS::Exmo_NS::ViaCurl_NS {
    using namespace TB_NS::Trader_NS::Interaction_NS;
    ExmoInteraction::ExmoInteraction(CommandCode i_commandCode, ExmoExchange& i_exmoEchange, ULONG i_nonce)
        : InteractI(i_commandCode)
        , m_exmoEchange(i_exmoEchange) {
        m_request += ToStr(i_nonce);
    }

    void ExmoInteraction::prepare(TradePair& io_tradePair) {
        auto recognize = [](const CurrencyCode& i_currency) -> Str::CR {
            static std::map<CurrencyID, Str> idToValue{
                { CurrencyID::BTC, "BTC" },
                { CurrencyID::RUB, "RUB" },
                { CurrencyID::USD, "USD" },
            };
            for (const auto& [id, value] : idToValue)
                if (i_currency == id)
                    return value;

            throw TB_EXEPT["EXMO"]["Builds"]["Unsupported Currency"]("{type:}", ToStr(i_currency)) << TB_LOCATION;
        };
        io_tradePair.view = recognize(io_tradePair._1) + "_" + recognize(io_tradePair._2);
        m_request += Str{ "&pair={1}" }.format("{1}", io_tradePair.view);
    }

    void ExmoInteraction::prepare(RequestLimit& io_quantityLimits) {
        if (m_commandCode == CommandID::GET_ORDER_BOOK) {
            static RequestLimit::ValueType min{ 1 }, def{ 100 }, max{ 1000 };
            if (!io_quantityLimits.isDefault())
                io_quantityLimits = def;
            else if (const auto& value = io_quantityLimits.getValue(); value < min || value > max) {
                TB_NS::Error_NS::Exception::ReplaceUnits ru{
                    { "{min:}", ToStr(min) },
                    { "{max:}", ToStr(max) },
                    { "{requested:}", ToStr(value) },
                    { "{id:}", ToStr(m_commandCode) },
                };
                throw TB_EXEPT["EXMO"]["API"]["REST"]["Violation"]["Limit"](std::move(ru)) << TB_LOCATION;
            }
        } else
            throw TB_EXEPT["EXMO"]["API"]["REST"]["Unregistered"]("{id:}", ToStr(m_commandCode)) << TB_LOCATION;

        m_request += Str{ "&limit={1}" }.format("{1}", ToStr(io_quantityLimits));
    }

    Json ExmoInteraction::interact() {
        return m_exmoEchange.request(m_commandCode, m_request);
    }
} // namespace TB_NS::Exmo_NS::ViaCurl_NS
