#pragma once

#include <trader/interactions/interaction.hpp>

#include <curl_adaptor/http.hpp>


namespace TB_NS::Exmo_NS {
    using namespace Trader_NS::Interaction_NS;
    class API : public ExchangeI {
        Str m_publicKey;
        Str m_secretKey;

        Str m_url{ "api.exmo.com/v1/"s };
        ULONG m_nonce{ static_cast<ULONG>(std::time(nullptr)) };
        CurlAdapter_NS::Http_NS::Connection m_connection{ CurlAdapter_NS::Http_NS::Connection() };

        public:
        API(Str i_public_key, Str i_secret_key);

#pragma region ExchangeI
        ExchangeCode getID() const noexcept override;
        void check(CommandID command_id, CurrencyPair& io_pair) const override;
        void check(CommandID command_id, Limit& io_limit) const override;
        Json interact(CommandID command_id, RequestedParameters::CR i_params) override;
#pragma endregion

        Json call(Str::CR i_method, Str::CR i_params);

        static Str build(LStrs::CR i_params);
    };
} // namespace TB_NS::Exmo_NS
