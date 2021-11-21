// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include <trader/interactions/interaction.hpp>

#include <curl_adaptor/http.hpp>

namespace TB_NS::Exmo_NS::ViaCurl_NS {
    using namespace Trader_NS::Interaction_NS;

    class ExmoExchange : public ExchangeI {
        Str m_publicKey;
        Str m_secretKey;

        Str m_url{ "api.exmo.com/v1/"s };
        mutable std::atomic<ULONG> m_nonce{ static_cast<ULONG>(std::time(nullptr)) };

        TB_PUBLIC_PRS(ExmoExchange);
        ExmoExchange(Str i_public_key, Str i_secret_key);

#pragma region ExchangeI
        ExchangeCode code() const noexcept override;
        InteractI::SP createIteractions(CommandCode i_commandCode) override;
#pragma endregion

        Str::CR method(CommandCode i_commandCode) const;

        Json request(CommandCode i_commandCode, Str::CR i_params);

        // TODO: should be removed
        Json call(Str::CR i_method, Str::CR i_params);
        static Str build_dd(LStrs::CR i_params);
    };

    class ExmoInteraction : public InteractI {
        Str m_request{ "nonce=" };
        ExmoExchange& m_exmoEchange;

        public:
        ExmoInteraction(CommandCode i_commandCode, ExmoExchange& i_exmoEchange, ULONG i_nonce);

#pragma region InteractI
        void prepare(TradePair& io_tradePair) override;
        void prepare(QuantityLimit& io_quantityLimits) override;
        Json interact() override;
#pragma endregion
    };
} // namespace TB_NS::Exmo_NS::ViaCurl_NS
