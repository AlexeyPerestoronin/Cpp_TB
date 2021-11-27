// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include "currency_id_code.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    // brief: presents one trade-pair for an Exchange platform
    struct TradePair : StrI {
        TB_PRS(TradePair);
        CurrencyCode _1{ CurrencyID::UNSUPPORTED };
        CurrencyCode _2{ CurrencyID::UNSUPPORTED };

        TradePair() noexcept = default;

        TradePair(CurrencyID i_1, CurrencyID i_2) noexcept
            : _1{ i_1 }
            , _2{ i_2 } {}

#pragma region StrI
        bool fromStr(Str::CR i_str) noexcept override final;
        Str toStr() const noexcept override final;
#pragma endregion
    };
} // namespace TB_NS::Trader_NS::Interaction_NS
