// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include <common/common.hpp>

namespace TB_NS::Trader_NS::Interaction_NS {
    // brief: collection of commands' ID(s) to an exchange platform
    enum class CommandID : uint8_t {
        UNSUPPORTED,
        GET_ORDER_BOOK,
    };

    // brief: converts a string to a command's ID
    void ToCommandIDFromStr(CommandID& i_value, Str::CR i_str) noexcept;

    // brief: converts a command's ID to a string
    Str ToStrfromCommandID(const CommandID& i_value) noexcept;

    // brief: the alias based extension for commands' ID(s)
    using CommandCode = AliasFor<CommandID, &ToCommandIDFromStr, &ToStrfromCommandID>;
} // namespace TB_NS::Trader_NS::Interaction_NS
