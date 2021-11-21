#pragma once

#include <trader/interactions/interaction.hpp>

#include <curl_adaptor/http.hpp>


namespace TB_NS::Exmo_NS {
    using namespace Trader_NS::Interaction_NS;
    class API : public ExchangeI {
        Str m_publicKey;
        Str m_secretKey;

        Str m_url{ "api.exmo.com/v1/"s };
        mutable ULONG m_nonce{ static_cast<ULONG>(std::time(nullptr)) };
        CurlAdapter_NS::Http_NS::Connection m_connection{ CurlAdapter_NS::Http_NS::Connection() };

        void check(CommandCode i_commandCode, TradePair& io_pair) const;
        void check(CommandCode i_commandCode, Limit& io_limit) const;
        void check(CommandCode i_commandCode, RequestedParameters& i_params) const;

        Str method(CommandCode i_commandCode) const;

        Str build(TradePair::CR i_pair) const;
        Str build(Limit::CR i_limit) const;
        Str build(RequestedParameters::CR i_params) const;

        public:
        API(Str i_public_key, Str i_secret_key);

#pragma region ExchangeI
        ExchangeCode code() const noexcept override;
        Json interact(CommandCode i_commandCode, RequestedParameters i_params) override;
#pragma endregion

        Json call(Str::CR i_method, Str::CR i_params);

        static Str build_dd(LStrs::CR i_params);
    };
} // namespace TB_NS::Exmo_NS
