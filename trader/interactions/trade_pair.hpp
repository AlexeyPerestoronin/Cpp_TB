// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include "currency_id_code.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    struct TradePair : StrI {
        TB_PRS(TradePair);
        CurrencyCode _1{};
        CurrencyCode _2{};

        TradePair(CurrencyID i_1, CurrencyID i_2)
            : _1{ i_1 }
            , _2{ i_2 } {}

#pragma region StrI
        bool from(Str::CR i_str) noexcept override final;
        Str to() const noexcept override final;
#pragma endregion
    };
} // namespace TB_NS::Trader_NS::Interaction_NS
