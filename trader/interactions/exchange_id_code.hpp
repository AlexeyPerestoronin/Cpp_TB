// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include <common/common.hpp>

namespace TB_NS::Trader_NS::Interaction_NS {
    // brief: exchange platforms' ID(s) collection
    enum class ExchangeID : uint8_t {
        UNSUPPORTED,
        EXMO,
    };

    // brief: converts a string to an exchange platform's ID
    bool ToExchangeIDFromStr(ExchangeID& i_value, Str::CR i_str) noexcept;

    // brief: converts an exchange platform's ID to a string
    Str ToStrFromExchangeID(const ExchangeID& i_value) noexcept;

    // brief: the alias based extension for exchange platforms' ID(s)
    using ExchangeCode = AliasFor<ExchangeID, &ToExchangeIDFromStr, &ToStrFromExchangeID>;
} // namespace TB_NS::Trader_NS::Interaction_NS
