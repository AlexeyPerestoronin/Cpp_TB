// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include "../command_id_code.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    namespace {
        static std::vector<std::pair<Str, CommandID>> NameToCode{
            { "CommandID::UNSUPPORTED", CommandID::UNSUPPORTED },
            { "CommandID::GET_ORDER_BOOK", CommandID::GET_ORDER_BOOK },
            { "CommandID::GET_PAIR_SETTINGS", CommandID::GET_PAIR_SETTINGS },
        };
    } // namespace

    bool ToCommandIDFromStr(CommandID& i_value, Str::CR i_str) noexcept {
        for (const auto& [name, code] : NameToCode)
            if (name == i_str) {
                i_value = code;
                return true;
            }
        i_value = CommandID::UNSUPPORTED;
        return false;
    }

    Str ToStrfromCommandID(const CommandID& i_value) noexcept {
        for (const auto& [name, code] : NameToCode)
            if (code == i_value)
                return name;
        return "CommandID::UNSUPPORTED";
    }
} // namespace TB_NS::Trader_NS::Interaction_NS
