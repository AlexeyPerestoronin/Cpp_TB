// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include <common/common.hpp>

namespace TB_NS::Trader_NS::Interaction_NS {
    // brief: the collections of ID(s) of currencies
    enum class CurrencyID : uint8_t {
        UNSUPPORTED,
        UNSUPPORTED3,
        // fiat
        USD,
        RUB,
        // crypto
        BTC,
    };

    // brief: converts a string to a currency's ID
    bool ToCurrencyIDFromStr(CurrencyID& i_value, Str::CR i_str) noexcept;

    // brief: converts a currency's ID to a string
    Str ToStrFromCurrencyID(const CurrencyID& i_value) noexcept;

    // brief: the alias based extension for currencies' ID(s)
    using CurrencyCode = AliasFor<CurrencyID, &ToCurrencyIDFromStr, &ToStrFromCurrencyID>;
} // namespace TB_NS::Trader_NS::Interaction_NS
